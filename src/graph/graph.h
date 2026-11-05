#pragma once

// Include the raylib libraries
#include "external/raylib/include/raylib.h"

// Dear ImGui headers
#include "external/imgui/imgui.h"

// Include the matrix library
#include "src/math/matrix.h"

#include <vector>
#include <cmath>
#include <string>

struct Curve {
    std::vector<Vector2> points;
    Vector2 startPoint;
    Color color;
    bool enabled = true;
    int id = 0;
};

class Graph {
    public:
        Graph(Vector2 initialScale = Vector2({40.0f, 30.0f}));
        
        void draw(const Math::Matrix& A);   // Renders all the curves to the graph
        void clear();                       // Clears all the curves from graph

        // Controls the scale of the graph
        Vector2 getBounds() const { return scale; };
        void setBounds(float x, float y) { scale = Vector2({x, y}); };
        
        // Creates a new curve on the graph
        void createNewCurve(Vector2 t0, const Math::Matrix& A, Color color = GetColor(0xff1313));

        Vector2 graphToScreen(ImVec2 winPos, ImVec2 winSize, Vector2 worldPos) const;
        Vector2 screenToGraph(ImVec2 winPos, ImVec2 winSize, Vector2 screenPos) const;

        std::vector<Curve> getCurves() const;
        void setCurveEnabled(int curvedIndex, bool enabled);
        void setCurveColor(int curvedIndex, Color col);
        void removeCurve(int curveIndex);

    private:
        int NEW_CURVE_ID = 0;

        Vector2 scale = Vector2({ 1.0f, 1.0f });
        std::vector<Curve> curves;

        const float flowArrowSpacing = 2.0f;

        Math::Matrix systemODE(const Math::Matrix& x, const Math::Matrix& A) const;
        Math::Matrix rk4Step(const Math::Matrix& x, const Math::Matrix& A, float dt) const;

        // Calculates points along a given solution of the differential equation
        Curve computeCurve(Vector2 t0, const Math::Matrix& A, int steps, float delta, Color color);

        // Gets the direction of "motion" of the graph at a given point t0
        Vector2 computeFlowDirection(Vector2 t0, const Math::Matrix& A) const;

        void drawArrow(ImVec2 winPos, ImVec2 winSize, Vector2 pos, Vector2 scaledDir, ImDrawList* drawList, ImU32 color, bool drawTail = true) const;
};