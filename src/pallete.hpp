#pragma once
#include "button.hpp"
#include "widgetManager.hpp"
#include "tool.hpp"


class ColorPallete : public WidgetManager
{
private:
    unsigned class_button_size = 30;
    unsigned class_button_distance = 10;

    Panel class_backgroud;

    shared_ptr<Widget> class_active_color; 
    shared_ptr<Widget> class_candidate_for_active_color;

    void setActiveColor(const shared_ptr<Widget>& color_ptr) {
        class_active_color = color_ptr;

        std::static_pointer_cast<ColorButton>(WidgetManager::arrayOfWidgets().front())->setColor(
            std::static_pointer_cast<ColorButton>(class_active_color)->color()
        );
    }


public:
    ColorPallete(const Vector& position) : class_backgroud{position - Vector{class_button_distance / 2,    
        class_button_distance / 2}, 0, 0, sf::Color{86, 83, 92}} {

        const std::vector<sf::Color> color = {
            sf::Color::Black,
            sf::Color::Blue,
            sf::Color::Cyan,
            sf::Color::Green,
            sf::Color::Magenta,
            sf::Color::Red,
            sf::Color::White,
            sf::Color::Yellow
        };

        class_backgroud.setSize(
            class_button_size + class_button_distance, 
            (color.size() + 1) * (class_button_size + class_button_distance)
        );

        int num_of_color = 0;

        WidgetManager::addWidget(make_shared<ColorButton>(
            position + Vector{0, (num_of_color) * (class_button_size + class_button_distance)},
            class_button_size, 
            class_button_size,
            color[num_of_color]
        ));
        
        for (auto size = color.size(); num_of_color < size; ++num_of_color) {
            WidgetManager::addWidget(make_shared<ColorButton>(
                position + Vector{0, (num_of_color+1) * (class_button_size + class_button_distance)},
                class_button_size, 
                class_button_size,
                color[num_of_color]
            ));
        }

        class_candidate_for_active_color  = WidgetManager::arrayOfWidgets().front(); 
        setActiveColor(class_candidate_for_active_color);
    }

    void draw(sf::RenderWindow& window) const override {
        class_backgroud.draw(window);
        WidgetManager::draw(window);
    }

    bool mousePressed(sf::Vector2i position) override {
        for (const auto& button : class_widgets) {
            if (button->contains(position.x, position.y)) {
                class_candidate_for_active_color = button;
                return true;
            }
        }
        return false;
    }

    bool mouseReleased(sf::Vector2i position) override {
        if (class_candidate_for_active_color->contains(position)) {
            setActiveColor(class_candidate_for_active_color);
            return true;
        }
        return false;
    }

    unsigned color() const {
        auto rgb_color = std::static_pointer_cast<ColorButton>(class_active_color)->color();
        return ((0xff << 24) | (rgb_color.b << 16) | (rgb_color.g << 8) | rgb_color.r);
    }
};



inline void Pencil::mousePressed(sf::Uint32* texture, unsigned x_resolution, unsigned y_resolution, unsigned x, unsigned y) {
    P1 = Vector{x, y};
}

inline void Pencil::pressButton(sf::Uint32* texture, unsigned x_resolution, unsigned y_resolution, unsigned x, unsigned y) {
    P2 = Vector{x, y};

    if (P1 == P2) {
        texture[clamp(y * x_resolution + x, 0U, x_resolution * y_resolution - 1)] = class_pallete->color();
        return;
    }


    int delta_x = (P2.x() > P1.x()) ? 1 : -1; 
    int delta_y = (P2.y() > P1.y()) ? 1 : -1; 



    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            
            texture[clamp((y + dy) * x_resolution + (x + dx), 0U, x_resolution * y_resolution - 1)] = class_pallete->color();

            int coord_x = 0, coord_y = 0;
            for (double t = 0; t <= 1; t += 0.001) {
                coord_x = P1.x() + (P2.x() - P1.x()) * t;
                coord_y = P1.y() + (P2.y() - P1.y()) * t;
                if (y == 0 && dy < 0) continue;
                    texture[clamp((coord_y + dy) * x_resolution + (coord_x + dx),
                    0U, 
                    x_resolution * y_resolution - 1)] = class_pallete->color();
            }
        }
    }

    P1 = P2;

}


inline void Pencil::mouseReleased(sf::Uint32* texture, unsigned x_resolution, unsigned y_resolution, unsigned x, unsigned y) {
    P2 = Vector{x, y};

    if (P1 == P2) {
        texture[clamp(y * x_resolution + x, 0U, x_resolution * y_resolution - 1)] = class_pallete->color();
    }

}



// inline void Pencil::mousePressed(sf::Uint32* texture, unsigned x_resolution, unsigned y_resolution, unsigned x, unsigned y) { 
//     for (int dx = -1; dx <= 1; ++dx) {
//         for (int dy = -1; dy <= 1; ++dy) {
//             if (y == 0 && dy < 0) continue;
//             texture[clamp((y + dy) * x_resolution + (x + dx), 0U, x_resolution * y_resolution - 1)] = class_pallete->color();
//         }
//     }
// }

inline void Line::mousePressed(sf::Uint32* texture, unsigned x_resolution, unsigned y_resolution, unsigned x, unsigned y) {
    P1 = Vector{x, y};
}

inline void Line::mouseReleased(sf::Uint32* texture, unsigned x_resolution, unsigned y_resolution, unsigned x, unsigned y) {
    P2 = Vector{x, y};

    int delta_x = (P2.x() > P1.x()) ? 1 : -1; 
    int delta_y = (P2.y() > P1.y()) ? 1 : -1; 

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int coord_x = 0, coord_y = 0;
            for (double t = 0; t <= 1; t += 0.001) {
                coord_x = P1.x() + (P2.x() - P1.x()) * t;
                coord_y = P1.y() + (P2.y() - P1.y()) * t;
                if (coord_y == 0 && dy < 0) continue;
                    texture[clamp((coord_y + dy) * x_resolution + (coord_x + dx),
                    0U, 
                    x_resolution * y_resolution - 1)] = class_pallete->color();
            }
            // for (int coord_x = P1.x(); coord_x != P2.x(); coord_x += delta_x) {
            //     for (int coord_y = P1.y(); coord_y != P2.y(); coord_y += delta_y) {
            //         if (y == 0 && dy < 0) continue;
            //         if (contains(coord_x, coord_y)) {
            //             texture[clamp((coord_y + dy) * x_resolution + (coord_x + dx),
            //             0U, 
            //             x_resolution * y_resolution - 1)] = class_pallete->color();
            //         }
            //     }
            // }
        }
    }

}



class ToolPallete : public WidgetManager, public ToolManager
{
private:
    unsigned class_button_size = 30;
    unsigned class_button_distance = 10;

    Panel class_backgroud;

    shared_ptr<Widget> class_active_tool; 
    std::list<shared_ptr<Widget>>::iterator class_candidate_for_active_tool_ptr;

    void setActiveTool(const std::list<shared_ptr<Widget>>::iterator& tool_ptr) {
        class_active_tool = *tool_ptr;
        std::static_pointer_cast<TextureButton>(class_active_tool)->setThinkes(true);
        
        auto distance = std::distance(class_widgets.begin(), tool_ptr);
        ToolManager::setActiveTool(distance);
    }

    void resetActiveTool(const std::list<shared_ptr<Widget>>::iterator& tool_ptr) {
        std::static_pointer_cast<TextureButton>(class_active_tool)->setThinkes(false);
        setActiveTool(tool_ptr);
    }


public:
    ToolPallete(const Vector& position, unsigned x_resolution) : class_backgroud{position - Vector{class_button_distance / 2,    
        class_button_distance / 2}, 0, 0, sf::Color{86, 83, 92}} {

        const std::vector<std::string> img_path = {
            "../Textures/pencil.png",
            //"../Textures/dropper.png",
            "../Textures/line.png",
        };

        class_backgroud.setSize(
            (class_button_size + class_button_distance), 
            (img_path.size() + 1) * (class_button_size + class_button_distance)
        );

        int num_of_instr = 0;
        for (auto size = img_path.size(); num_of_instr < size; ++num_of_instr) {
            WidgetManager::addWidget(make_shared<TextureButton>(
                position + Vector{0, num_of_instr * (class_button_size + class_button_distance)},
                class_button_size, 
                class_button_size,
                img_path[num_of_instr]
            ));
        }

        // WidgetManager::addWidget(make_shared<ColorButton>(
        //     position + Vector{0, num_of_instr * (class_button_size + class_button_distance)},
        //     class_button_size, 
        //     class_button_size,
        //     sf::Color::Black
        // ));

        WidgetManager::addWidget(make_shared<ColorPallete>(
            position + Vector{0, num_of_instr * (class_button_size + class_button_distance)}
        ));

        const auto& color_pallete = std::static_pointer_cast<ColorPallete>(class_widgets.back());
        ToolManager::addTool(make_shared<Pencil>(color_pallete, x_resolution));
        ToolManager::addTool(make_shared<Line>(color_pallete, x_resolution));

        class_candidate_for_active_tool_ptr = class_widgets.begin(); 
        setActiveTool(class_candidate_for_active_tool_ptr);
    }

    void draw(sf::RenderWindow& window) const override {
        ToolManager::activeTool()->draw(window);
        class_backgroud.draw(window);
        WidgetManager::draw(window);
    }

    bool mousePressed(sf::Vector2i position) override {

        auto status = class_widgets.back()->mousePressed(position);

        for (auto button_ptr = class_widgets.begin(); button_ptr != class_widgets.end(); ++button_ptr) {
            if ((*button_ptr)->contains(position.x, position.y)) {
                class_candidate_for_active_tool_ptr = button_ptr;
            }
        }
        return status;
    }

    bool mouseReleased(sf::Vector2i position) override {
        
        auto status = class_widgets.back()->mouseReleased(position);

        if ((*class_candidate_for_active_tool_ptr)->contains(position)) {
            if (std::dynamic_pointer_cast<TextureButton>(*class_candidate_for_active_tool_ptr) != nullptr) {
                resetActiveTool(class_candidate_for_active_tool_ptr);
            }
        }
        return status;
    }
};
