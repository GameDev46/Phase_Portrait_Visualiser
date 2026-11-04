#include "graph.h"

Graph::Graph(Vector2 initialScale) : scale(initialScale) {}

void Graph::draw(const Math::Matrix& A) {
    ImVec2 winPos = ImGui::GetWindowPos();
    ImVec2 winSize = ImGui::GetContentRegionAvail();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Clip drawing strictly inside the ImGui window boundaries
    drawList->PushClipRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y), true);

    // 2. Compute Flow Field & Normalization
    std::vector<std::pair<Vector2, Vector2>> flowVectors;
    float maxMagSqr = 0.0001f;

    for (float x = -scale.x; x < scale.x; x += flowArrowSpacing) {
        for (float y = -scale.y; y < scale.y; y += flowArrowSpacing) {
            Vector2 pos = {x, y};
            Vector2 flowDir = computeFlowDirection(pos, A);

            float magSqr = (flowDir.x * flowDir.x) + (flowDir.y * flowDir.y);
            maxMagSqr = std::max(maxMagSqr, magSqr);

            flowVectors.push_back({pos, flowDir});
        }
    }

    float maxMag = std::sqrt(maxMagSqr);
    maxMag /= flowArrowSpacing;

    // 3. Render Vector Field Arrows
    for (const auto& [pos, flowDir] : flowVectors) {
        Vector2 scaledDir = { flowDir.x / maxMag, flowDir.y / maxMag };
        drawArrow(winPos, winSize, pos, scaledDir, drawList, IM_COL32(50, 50, 50, 255));
    }

    // Draw the x,y axis
    Vector2 origin = graphToScreen(winPos, winSize, {0.0f, 0.0f});
    drawList->AddLine(ImVec2(winPos.x, origin.y), ImVec2(winPos.x + winSize.x, origin.y), IM_COL32(70, 70, 70, 255), 1.0f);
    drawList->AddLine(ImVec2(origin.x, winPos.y), ImVec2(origin.x, winPos.y + winSize.y), IM_COL32(70, 70, 70, 255), 1.0f);

    // Draw the eigenvectors
    std::vector<Math::EigenPair> eigenPairs = A.get2x2EigenVectorsAndValues();

    int index = 1;

    for (Math::EigenPair eigenPair : eigenPairs) {
        Vector2 vec = { eigenPair.eigenVector[0], eigenPair.eigenVector[1] };
        float lambda = eigenPair.eigenValue;

        // Skip lambdas of zero
        if (lambda == 0.0f) continue;

        float mag = std::sqrt(vec.x*vec.x + vec.y*vec.y);
        vec = { (vec.x / mag) * 3.0f, (vec.y / mag) * 3.0f };

        drawArrow(
            winPos,
            winSize,
            {0.0f, 0.0f},
            vec,
            drawList,
            IM_COL32(80, 80, 80, 255)
        );

        Vector2 lambdaPos = graphToScreen(winPos, winSize, { vec.x * 1.1f, vec.y * 1.1f });

        drawList->AddText(
            ImVec2(lambdaPos.x, lambdaPos.y),
            IM_COL32(80, 80, 80, 255),
            ("v" + std::to_string(index)).c_str()
        );

        index++;
    }

    // Draw the solution curves
    for (const Curve& curve : curves) {
        if (curve.points.size() < 2) continue;

        Vector2 lastArrowPos = graphToScreen(winPos, winSize, curve.points[0]);

        for (size_t i = 1; i < curve.points.size(); i++) {
            Vector2 p1 = graphToScreen(winPos, winSize, curve.points[i - 1]);
            Vector2 p2 = graphToScreen(winPos, winSize, curve.points[i]);

            drawList->AddLine(
                ImVec2(p1.x, p1.y), 
                ImVec2(p2.x, p2.y), 
                IM_COL32(curve.color.r, curve.color.g, curve.color.b, curve.color.a), 
                2.0f
            );

            // Draw spaced out direction arrows
            if (std::abs(lastArrowPos.x - p1.x) + std::abs(lastArrowPos.y - p1.y) > 100.0f) {
                lastArrowPos = p1;
                
                Vector2 flowDir = computeFlowDirection(curve.points[i], A);
                float mag = std::sqrt(flowDir.x*flowDir.x + flowDir.y*flowDir.y);

                Vector2 scaledDir = { (flowDir.x / mag) * 2.5f, (flowDir.y / mag) * 2.5f };

                drawArrow(
                    winPos,
                    winSize,
                    curve.points[i],
                    scaledDir,
                    drawList,
                    IM_COL32(curve.color.r, curve.color.g, curve.color.b, curve.color.a),
                    false
                );
            }
        }

        Vector2 t0 = graphToScreen(winPos, winSize, curve.startPoint);

        // Draw small dot at t0
        drawList->AddCircleFilled(
            ImVec2(t0.x, t0.y),
            3.5f,
            IM_COL32(curve.color.r, curve.color.g, curve.color.b, curve.color.a)
        );

        Vector2 flowDir = computeFlowDirection(t0, A);
        Vector2 textGraphPos = { curve.startPoint.x + flowDir.y * 0.05f, curve.startPoint.y - flowDir.x * 0.05f };
        Vector2 t0Text = graphToScreen(winPos, winSize, textGraphPos);

        drawList->AddText(
            ImVec2(t0Text.x, t0Text.y),
            IM_COL32(curve.color.r, curve.color.g, curve.color.b, curve.color.a),
            "t0"
        );
    }

    drawList->PopClipRect();
}

void Graph::clear() {
    // Clear all the curves
    curves.clear();
}

Vector2 Graph::graphToScreen(ImVec2 winPos, ImVec2 winSize, Vector2 worldPos) const {
    float centerX = winPos.x + winSize.x * 0.5f;
    float centerY = winPos.y + winSize.y * 0.5f;

    float xScale = winSize.x / (scale.x * 2.0f);
    float yScale = winSize.y / (scale.y * 2.0f);
    
    return { centerX + worldPos.x * xScale, centerY - worldPos.y * yScale };
};

Vector2 Graph::screenToGraph(ImVec2 winPos, ImVec2 winSize, Vector2 screenPos) const {
    float centerX = winPos.x + winSize.x * 0.5f;
    float centerY = winPos.y + winSize.y * 0.5f;

    return { ((screenPos.x - centerX) * scale.x) / (winSize.x * 0.5f), ((centerY - screenPos.y) * scale.y) / (winSize.y * 0.5f) };
}

Math::Matrix Graph::systemODE(const Math::Matrix& x, const Math::Matrix& A) const {
    return A * x;
}

Math::Matrix Graph::rk4Step(const Math::Matrix& x, const Math::Matrix& A, float dt) const {
    Math::Matrix k1 = systemODE(x, A);
    Math::Matrix k2 = systemODE(x + (k1 * (dt * 0.5f)), A);
    Math::Matrix k3 = systemODE(x + (k2 * (dt * 0.5f)), A);
    Math::Matrix k4 = systemODE(x + (k3 * dt), A);

    return x + (k1 + (k2 * 2.0f) + (k3 * 2.0f) + k4) * (dt / 6.0f);
}

void Graph::createNewCurve(Vector2 t0, const Math::Matrix& A, Color color) {
    // Calculate points along the curve
    Curve curve = computeCurve(t0, A, 500, 0.01f, color);

    // Store the curve ready to be drawn
    curves.push_back(curve);
}

Curve Graph::computeCurve(Vector2 t0, const Math::Matrix& A, int steps, float delta, Color color) const {
    Curve curve;
    curve.startPoint = t0;
    curve.color = color;

    // Forward approximation
    Math::Matrix x(2, 1);
    x(0,0) = t0.x;
    x(1,0) = t0.y;

    std::vector<Vector2> forwardPoints;

    for (int i = 0; i < steps; i++) {
        forwardPoints.push_back({ x(0,0), x(1,0) });
        x = rk4Step(x, A, delta);
        
        // If the curve goes very far out of view then stop drawing
        if (std::abs(x(0,0)) > scale.x * 5.0f || std::abs(x(1,0)) > scale.y * 5.0f) break;
    }

    // Backward approximation
    x(0,0) = t0.x;
    x(1,0) = t0.y;

    std::vector<Vector2> backwardPoints;

    for (int i = 0; i < steps; i++) {
        x = rk4Step(x, A, -delta);
        backwardPoints.push_back({ x(0,0), x(1,0) });
        
        // If the curve goes out of view then stop drawing
        if (std::abs(x(0,0)) > scale.x || std::abs(x(1,0)) > scale.y) break;
    }

    // Clear the curve points
    curve.points.clear();

    // Combine the forward and backward points
    for (int i = backwardPoints.size()-1; i >= 0; i--) {
        curve.points.push_back(backwardPoints[i]);
    }

    for (int i = 0; i < forwardPoints.size(); i++) {
        curve.points.push_back(forwardPoints[i]);
    }

    return curve;
}

Vector2 Graph::computeFlowDirection(Vector2 t0, const Math::Matrix& A) const {
    Math::Matrix x(2, 1);
    x(0,0) = t0.x;
    x(1,0) = t0.y;

    Math::Matrix y = rk4Step(x, A, 0.01f);
    
    return { y(0,0) - x(0,0), y(1,0) - x(1,0) };
}

void Graph::drawArrow(ImVec2 winPos, ImVec2 winSize, Vector2 pos, Vector2 scaledDir, ImDrawList* drawList, ImU32 color, bool drawTail) const {
    Vector2 scaledDirPerp = { scaledDir.y, -scaledDir.x };
    
    Vector2 endPos = { pos.x + scaledDir.x, pos.y + scaledDir.y };

    float triangleSize = 0.3f;
    if (!drawTail) endPos = pos;

    Vector2 triangleBase = { endPos.x - scaledDir.x * triangleSize, endPos.y - scaledDir.y * triangleSize };
    Vector2 triangleCorner1 = { triangleBase.x + scaledDirPerp.x * triangleSize * 0.5f, triangleBase.y + scaledDirPerp.y * triangleSize * 0.5f };
    Vector2 triangleCorner2 = { triangleBase.x - scaledDirPerp.x * triangleSize * 0.5f, triangleBase.y - scaledDirPerp.y * triangleSize * 0.5f };

    Vector2 p1 = graphToScreen(winPos, winSize, pos);
    Vector2 p2 = graphToScreen(winPos, winSize, triangleBase);

    Vector2 t1 = graphToScreen(winPos, winSize, triangleCorner1);
    Vector2 t2 = graphToScreen(winPos, winSize, triangleCorner2);
    Vector2 t3 = graphToScreen(winPos, winSize, endPos);

    // Draw arrow shaft
    if (drawTail) drawList->AddLine(ImVec2(p1.x, p1.y), ImVec2(p2.x, p2.y), color, 1.8f);

    drawList->AddTriangleFilled(
        ImVec2(t3.x, t3.y),
        ImVec2(t1.x, t1.y),
        ImVec2(t2.x, t2.y),
        color
    );
}