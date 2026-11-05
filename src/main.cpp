// Include the raylib libraries
#include "external/raylib/include/raylib.h"
#include "external/raylib/include/raymath.h"

// Dear ImGui headers
#include "external/imgui/imgui.h"

#include "rlImGui.h"
#include "imgui_impl_raylib.h"

// Maths and graph management
#include "math/matrix.h"
#include "graph/graph.h"

// Default C++ libraries
#include <vector>
#include <string>

// Program entry point
int main()
{
    // Raylib window height and width
    const int SCREEN_WIDTH = 1200;
    const int SCREEN_HEIGHT = 800;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Linear System Phase Portrait");

    rlImGuiSetup(true);

    // Enable docking and viewports
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    io.ConfigDockingWithShift = false;

    //Image windowIcon = LoadImage("images/icon.png");
    //SetWindowIcon(windowIcon);

    // Set the max FPS
    SetTargetFPS(100);

    // Load a 1x1 white texture
    Texture2D whiteTex = LoadTextureFromImage(GenImageColor(1, 1, WHITE));

    // Create a 2x2 zero matrix
    Math::Matrix A(2, 2);
    bool AChanged = false;

    // Create the phase portrait graph
    Graph phaseGraph;

    // Main window render loop
    while (!WindowShouldClose())
    {
        const float delta = GetFrameTime();

        BeginDrawing();
            ClearBackground(BLACK);

            // Start ImGui Conent
            rlImGuiBegin();

                // Create the main "dockspace node"
                ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | 
                                                ImGuiWindowFlags_NoTitleBar |
                                                ImGuiWindowFlags_NoCollapse |
                                                ImGuiWindowFlags_NoResize |
                                                ImGuiWindowFlags_NoMove |
                                                ImGuiWindowFlags_NoBringToFrontOnFocus |
                                                ImGuiWindowFlags_NoNavFocus |
                                                ImGuiWindowFlags_NoBackground;

                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->WorkPos);
                ImGui::SetNextWindowSize(viewport->WorkSize);

                ImGui::Begin("DockSpaceHost", nullptr, window_flags);
                    ImGuiID dockspace_id = ImGui::GetID("MainDockspace");
                    ImGui::DockSpace(dockspace_id, ImVec2(0,0), ImGuiDockNodeFlags_PassthruCentralNode);
                ImGui::End();

                // 
                ImGui::Begin("Phase Diagram Plot");

                    if (AChanged) {
                        AChanged = false;
                        //phaseGraph.clear();
                    }

                    phaseGraph.draw(A);
                    
                    bool hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_None);
                    bool leftDown = ImGui::IsMouseClicked(ImGuiMouseButton_Left);

                    if (hovered && leftDown) {
                        ImVec2 winPos = ImGui::GetWindowPos();
                        ImVec2 winSize = ImGui::GetContentRegionAvail();

                        ImVec2 mousePos = ImGui::GetMousePos();
                        Vector2 worldClick = phaseGraph.screenToGraph(winPos, winSize, { mousePos.x, mousePos.y });

                        Color randomColor = ColorFromHSV((float)GetRandomValue(0, 360), 0.8f, 0.95f);
                        phaseGraph.createNewCurve(worldClick, A, randomColor);
                    }
                
                ImGui::End();

                ImGui::Begin("Curves");

                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.25f, 0.25f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.25f, 0.25f, 1.0f));

                    if (ImGui::Button("Remove All Curves", { 250, 30 })) phaseGraph.clear();

                    ImGui::PopStyleColor();
                    ImGui::PopStyleColor();
                    ImGui::PopStyleColor();

                    std::vector<Curve> curves = phaseGraph.getCurves();
                    for (int i = 0; i < curves.size(); i++) {

                        ImGui::PushID(curves[i].id);

                        ImVec4 headerCol = ImVec4(curves[i].color.r / 255.0f, curves[i].color.g / 255.0f, curves[i].color.b / 255.0f, curves[i].color.a / 255.0f);
                        ImGui::PushStyleColor(ImGuiCol_Header, headerCol);
                        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, headerCol);
                        ImGui::PushStyleColor(ImGuiCol_HeaderActive, headerCol);

                        if (ImGui::CollapsingHeader(("Curve " + std::to_string(i)).c_str())) {

                            int curveEnabled = curves[i].enabled ? 1 : 0;

                            ImGui::PushItemWidth(100);
                            if (ImGui::SliderInt("Show Curve", &curveEnabled, 0, 1)) phaseGraph.setCurveEnabled(i, curveEnabled == 1);
                            ImGui::PopItemWidth();
                            
                            ImGui::Text("");
                            ImGui::Separator();
                            ImGui::Text("");

                            ImGui::Text("t0: (%.2f, %.2f)", curves[i].startPoint.x, curves[i].startPoint.y);

                            ImGui::Text("");
                            ImGui::Separator();
                            ImGui::Text("");
                            
                            ImGui::PushItemWidth(130);

                            int red = curves[i].color.r;
                            if (ImGui::InputInt("Red", &red)) curves[i].color.r = std::min(std::max(red, 0), 255);

                            int green = curves[i].color.g;
                            if (ImGui::InputInt("Green", &green)) curves[i].color.g = std::min(std::max(green, 0), 255);

                            int blue = curves[i].color.b;
                            if (ImGui::InputInt("Blue", &blue)) curves[i].color.b = std::min(std::max(blue, 0), 255);

                            int alpha = curves[i].color.a;
                            if (ImGui::InputInt("Alpha", &alpha)) curves[i].color.a = std::min(std::max(alpha, 0), 255);
                            
                            // Update the stored curve color
                            phaseGraph.setCurveColor(i, curves[i].color);

                            ImGui::PopItemWidth();

                            ImGui::Text("");
                            ImGui::Separator();
                            ImGui::Text("");

                            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.25f, 0.25f, 1.0f));
                            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.25f, 0.25f, 1.0f));

                            if (ImGui::Button("Remove Curve", { 250, 30 })) phaseGraph.removeCurve(i);

                            ImGui::PopStyleColor();
                            ImGui::PopStyleColor();
                            ImGui::PopStyleColor();
                        }

                        ImGui::PopStyleColor();
                        ImGui::PopStyleColor();
                        ImGui::PopStyleColor();

                        ImGui::PopID();
                    }

                ImGui::End();

                ImGui::Begin("System Settings");
                    
                    ImGui::Text("dx/dt = Ax");

                    ImGui::Text("");
                    ImGui::Separator();
                    ImGui::Text("");
                    
                    ImGui::Text("Matrix A:");
                    
                    ImGui::PushItemWidth(80);
                    AChanged = AChanged || ImGui::InputFloat("a", &A(0, 0)); ImGui::SameLine();
                    AChanged = AChanged || ImGui::InputFloat("b", &A(0, 1));
                    AChanged = AChanged || ImGui::InputFloat("c", &A(1, 0)); ImGui::SameLine();
                    AChanged = AChanged || ImGui::InputFloat("d", &A(1, 1));
                    ImGui::PopItemWidth();

                    ImGui::Text("");
                    ImGui::Separator();
                    ImGui::Text("");
                    
                    ImGui::Text("Eigenvectors and Eigenvalues");
                    ImGui::Text("");

                    std::vector<Math::EigenPair> eigenPairs = A.get2x2EigenVectorsAndValues();

                    if (eigenPairs.size() == 0) ImGui::Text("No real eigenvectors");

                    int index = 1;
                    for (Math::EigenPair eigenPair : eigenPairs) {
                        float v1 = eigenPair.eigenVector[0];
                        float v2 = eigenPair.eigenVector[1];
                        float lambda = eigenPair.eigenValue;

                        // Skip lambdas of zero
                        if (lambda == 0.0f) continue;

                        ImGui::Text("v%d \nEigenvector: (%.2f, %.2f) \nEigenvalue: %.2f", index, v1, v2, lambda);
                        ImGui::Text("");

                        index++;
                    }

                ImGui::End();

            // End ImGui Content
            rlImGuiEnd();

		EndDrawing();
    }

    // Cleanup ImGui
    rlImGuiShutdown();

    // Close the window
    CloseWindow();

    // Exit the program
    return 0;
}