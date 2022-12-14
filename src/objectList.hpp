#pragma once
#include "widgetManager.hpp"
#include "button.hpp"
#include "main.hpp"
#include "console.hpp"

struct ObjInfo
{
    Vector position;
    unsigned width;
    unsigned height;
    int num_widgets = 0;
};


class WindowBody : public WidgetManager
{
private:
    unsigned class_panel_size = 15;
    sf::Color class_panel_color = sf::Color{190, 190, 190};

    ObjInfo class_info;

    std::list<std::shared_ptr<Widget>>::reverse_iterator it;

public:
    WindowBody(const ObjInfo& info) : WindowBody(info.position, info.width, info.height) {}
    WindowBody(const Vector& position, unsigned width, unsigned height, sf::Color color = sf::Color{86, 83, 92}) :
        class_info{position, width, height} {

        double koef_button_size = 0.7;
        
        WidgetManager::addWidget(make_shared<MovePanel>(
            position, 
            width,
            class_panel_size,
            class_panel_color,
            this
        ));

        WidgetManager::addWidget(make_shared<TextureButton>(
            position + Vector{width - class_panel_size * 0.5 * (1 + koef_button_size), class_panel_size * 0.5 * (1 - koef_button_size)},
            class_panel_size * koef_button_size,
            class_panel_size * koef_button_size,
            "../Textures/cross.png"
        ));
        WidgetManager::addWidget(make_shared<Panel>(
            position + Vector{0, class_panel_size}, 
            width,
            height,
            color
        ));


        class_info.num_widgets = WidgetManager::arrayOfWidgets().size();
    }

    ObjInfo info() { return class_info; }

    Vector position() const override {
        return class_info.position + Vector{0, class_panel_size}; 
    }
};


class NoMoveWindowBody : public WidgetManager
{
private:
    unsigned class_panel_size = 15;
    sf::Color class_panel_color = sf::Color{190, 190, 190};

    ObjInfo class_info;

public:
    NoMoveWindowBody(const ObjInfo& info) : NoMoveWindowBody(info.position, info.width, info.height) {}
    NoMoveWindowBody(const Vector& position, unsigned width, unsigned height, sf::Color color = sf::Color{86, 83, 92}) :
        class_info{position, width, height} {

        double koef_button_size = 0.7;
        
        WidgetManager::addWidget(make_shared<NoMovePanel>(
            position, 
            width,
            class_panel_size,
            class_panel_color,
            this
        ));

        WidgetManager::addWidget(make_shared<TextureButton>(
            position + Vector{width - class_panel_size * 0.5 * (1 + koef_button_size), class_panel_size * 0.5 * (1 - koef_button_size)},
            class_panel_size * koef_button_size,
            class_panel_size * koef_button_size,
            "../Textures/cross.png"
        ));
        WidgetManager::addWidget(make_shared<Panel>(
            position + Vector{0, class_panel_size}, 
            width,
            height,
            color
        ));

        class_info.num_widgets = WidgetManager::arrayOfWidgets().size();
    }

    ObjInfo info() { return class_info; }
};


class ObjectList;



class ObjectCutaway : public WindowBody 
{
private:
    ObjectList* class_obj_list;

    static ObjInfo createBodyInfo(const ObjInfo& list_info) {
        ObjInfo ret = {
            list_info.position + Vector{20, 20},
            list_info.width - 40,
            list_info.height - 40,
        };
        return ret;
    }
public:
    ObjectCutaway(ObjectList* obj_list);
        //WindowBody::addWidget()
};


class ObjectCreater : public NoMoveWindowBody
{
private:
    std::vector<shared_ptr<Console>> class_consoles;

    enum { LAMBERTIAN, METALL, DIELECTRIC, NO_MATERIAL }; 
    int class_material = NO_MATERIAL;
public:
    ObjectCreater(const Vector& position, unsigned width, unsigned height, sf::Color color) : NoMoveWindowBody{position, width, height, color} {
        
        auto text = make_shared<TextButton>(
            position + Vector{15, 30},
            0,
            20,
            "Material: ",
            sf::Color{0, 0, 0, 0}
        );
        text->setTextColor(sf::Color::White);
        NoMoveWindowBody::addWidget(text);

        unsigned drodown_button_sz = width - 120;

        auto material_list = make_shared<DropdownButton>(
            position + Vector{110, 25},
            drodown_button_sz,
            30,
            sf::Color{50, 50, 50}
        );

        auto lambertian_button = make_shared<TextButton>(
            position + Vector{110, 25},
            280,
            30,
            "lambertian",
            sf::Color{50, 50, 50}
        );
        lambertian_button->setTextColor(sf::Color::White);
        lambertian_button->setCharacterSize(20*0.8);
        lambertian_button->moveText(5, 5);
        material_list->addWidget(lambertian_button);

        auto metall_button = make_shared<TextButton>(
            position + Vector{110, 55},
            280,
            30,
            "metall",
            sf::Color{50, 50, 50}
        );
        metall_button->setTextColor(sf::Color::White);
        metall_button->setCharacterSize(20*0.8);
        metall_button->moveText(5, 5);
        material_list->addWidget(metall_button);

        auto dielectric_button = make_shared<TextButton>(
            position + Vector{110, 85},
            280,
            30,
            "dielectric",
            sf::Color{50, 50, 50}
        );
        dielectric_button->setTextColor(sf::Color::White);
        dielectric_button->setCharacterSize(20*0.8);
        dielectric_button->moveText(5, 5);
        material_list->addWidget(dielectric_button);

        auto x_text = make_shared<TextButton>(
            position + Vector{width/6 - 0.5*20*0.8, 70},
            0,
            20,
            "x",
            sf::Color{0, 0, 0, 0}
        );
        x_text->setTextColor(sf::Color::White);
        NoMoveWindowBody::addWidget(x_text);


        unsigned cns_sz = (width-40)/3;

        auto console_x = make_shared<Console>(
            position + Vector{10, 120},
            cns_sz,
            20,
            sf::Color{50, 50, 50},
            std::bind(std::mem_fn(&ObjectCreater::disable_consoles), this)
        );


        auto y_text = make_shared<TextButton>(
            position + Vector{width/2 - 0.5*20*0.8, 70},
            0,
            20,
            "y",
            sf::Color{0, 0, 0, 0}
        );
        y_text->setTextColor(sf::Color::White);
        NoMoveWindowBody::addWidget(y_text);


        auto console_y = make_shared<Console>(
            position + Vector{cns_sz + 20, 120},
            cns_sz,
            20,
            sf::Color{50, 50, 50},
            std::bind(std::mem_fn(&ObjectCreater::disable_consoles), this)
        );

        auto z_text = make_shared<TextButton>(
            position + Vector{5*width/6 - 0.5*20*0.8, 70},
            0,
            20,
            "z",
            sf::Color{0, 0, 0, 0}
        );
        z_text->setTextColor(sf::Color::White);
        NoMoveWindowBody::addWidget(z_text);

        auto console_z = make_shared<Console>(
            position + Vector{2 * cns_sz + 30, 120},
            cns_sz,
            20,
            sf::Color{50, 50, 50},
            std::bind(std::mem_fn(&ObjectCreater::disable_consoles), this)
        );

        class_consoles.push_back(console_x);
        class_consoles.push_back(console_y);
        class_consoles.push_back(console_z);
        NoMoveWindowBody::addWidget(console_x);
        NoMoveWindowBody::addWidget(console_y);
        NoMoveWindowBody::addWidget(console_z);

        NoMoveWindowBody::addWidget(material_list);

    }

private:
    void disable_consoles() {
        for (auto& cns: class_consoles) {
            cns->is_active_console = false;
        }
    }
    // shared_ptr<Console> consoles() {
    //     auto it = class_widgets.end();
    //     --it;
    //     --it;
    //     assert(typeid(**it) == typeid(Console));
    //     return std::static_pointer_cast<Console>(*it);
    // }
public:

    bool mousePressed(sf::Vector2i position) override {
        class_mouse_is_pressed = true;
        auto is_pressed = false;
        for (auto widget_it = class_widgets.rbegin(); widget_it != class_widgets.rend(); ++widget_it) {
            if (*widget_it != nullptr) {
                is_pressed = (*widget_it)->mousePressed(position);
                if (is_pressed) {
                    break;
                }
            }
        }
        return is_pressed;
    }
};

inline bool CreateObjButton::mouseReleased(sf::Vector2i position)
{
    if (contains(position.x, position.y) && class_is_pressed) {
        Vector new_position = class_widget_manager_ptr->position();
        new_position += Vector{10, 10};

        auto size = Button::size().x() - 20;

        class_widget_manager_ptr->addWidget(make_shared<ObjectCreater>(
            new_position, size, size, sf::Color{76, 73, 82}
        ));
        return true;
    }
    
    class_is_pressed = false;
    return false;
}


class ObjectList : public WindowBody
{
private:
    sf::Color class_panel_color = sf::Color{190, 190, 190};

    unsigned class_panel_size = 25;

    sf::Vector2i class_last_position;
    sf::Vector2i class_delta;
public:
    ObjectList(const Vector& position, unsigned width, unsigned height, WidgetManager* global_manager) : WindowBody{position, width, height} {
        WindowBody::addWidget(make_shared<CreateObjButton>(
            position + Vector{0, height - 25 + 15}, 
            width,
            25,
            "New Object",
            sf::Color{76, 73, 82},
            this   
        ));
    }

    bool mousePressed(sf::Vector2i position) override {
        class_mouse_is_pressed = true;
        auto is_pressed = false;
        for (auto widget_it = class_widgets.rbegin(); widget_it != class_widgets.rend(); ++widget_it) {
            if (*widget_it != nullptr) {
                is_pressed = (*widget_it)->mousePressed(position);
                if (is_pressed) {
                    break;
                }
            }
        }
        return is_pressed;
    }

};

ObjectCutaway::ObjectCutaway(ObjectList* obj_list) : WindowBody{createBodyInfo(obj_list->info())}, class_obj_list{obj_list} {
        //addWidget()
    }