//
//  Gui.cpp
//  PocketOSX
//
//  Created by Jeppe Nielsen on 01/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Gui.hpp"
#include "FontTextureSystem.hpp"
#include "FileReader.hpp"
#include <fstream>
#include "JsonDeserializer.hpp"
#include "VirtualTreeListSystem.hpp"
#include "VirtualTreeListSpawnerSystem.hpp"

using namespace Mini;

GameObject Gui::GetAtlas() { return atlas; }

void Gui::Initialize() {
    renderer = &scene->CreateSystem<RenderSystem>();
    textboxSystem = &scene->CreateSystem<TextBoxSystem>();
    touchSystem = &scene->CreateSystem<TouchSystem>();
    
    scene->CreateSystem<TransformHierarchy>();
    
    scene->CreateSystem<SpriteMeshSystem>();
    scene->CreateSystem<SpriteTextureSystem>();
    scene->CreateSystem<HierarchyOrder>();//.Order = 1000;
    scene->CreateSystem<DraggableSystem>();
    scene->CreateSystem<LayoutSystem>();
    scene->CreateSystem<ColorSystem>();
    scene->CreateSystem<DraggableMotionSystem>();
    scene->CreateSystem<VelocitySystem>();
    scene->CreateSystem<LimitableSystem>();
    //scene->CreateSystem<SelectedColorerSystem>();
    scene->CreateSystem<DroppableSystem>();
    scene->CreateSystem<DraggableMotionSystem>();
    scene->CreateSystem<LayoutSystem>();
    scene->CreateSystem<FontTextureSystem>();//->Order = -1;
    scene->CreateSystem<LabelMeshSystem>();
    scene->CreateSystem<TextBoxLabelSystem>();
    
    scene->CreateSystem<PanelDropSystem>();//->Order = -1;
    scene->CreateSystem<PanelSystem>();//->Order = -1;
    scene->CreateSystem<PanelAreaSystem>();//->Order = -1;
    scene->CreateSystem<TouchCancelSystem>();
    
    hoverSystem = &scene->CreateSystem<HoverSystem>();
    scene->CreateSystem<ScrollWheelMoverSystem>();
    scene->CreateSystem<VirtualTreeListSystem>();
    scene->CreateSystem<VirtualTreeListSpawnerSystem>();
}

void Gui::Setup(const std::string &atlasTexture, const std::string &atlasXml, const Rect& viewport) {

    auto atlasFile = FileReader::GetFile(atlasXml);
    std::ifstream file;
    file.open(atlasFile);
    JsonDeserializer serializer;
    atlas = scene->CreateObject();
    serializer.DeserializeObject(atlas, file);
    file.close();
    Texture& texture = atlas.AddComponent<TextureComponent>()->Texture();
    texture.LoadFromFile(atlasTexture);
    texture.DisableMipmapping();
    Setup(atlas, viewport);
}

void Gui::Setup(GameObject atlas, const Rect &viewport) {
    
    this->atlas = atlas;
    
    BoundingBox bounds(0, Vector3(viewport.width * 2.0f, viewport.height * 2.0f, 3000.0f));
    renderer->Octree().SetWorldBounds(bounds);
    touchSystem->Octree().SetWorldBounds(bounds);
    hoverSystem->Octree().SetWorldBounds(bounds);
    
    camera = scene->CreateObject();
    camera.AddComponent<Transform>()->Position = Vector3(0,0,1);
    Camera* cam = camera.AddComponent<Camera>();
    
    cam->Orthographic = true;
    cam->Near = 1.0f;
    cam->Far = 2.0f;
    cam->FieldOfView = 0.0f;
}

GameObject Gui::GetCamera() {
    return camera;
}

GameObject Gui::CreatePivot(GameObject parent) {
    return CreatePivot(parent, {0,0});
}

GameObject Gui::CreatePivot(GameObject parent, const Vector2& position) {
    GameObject pivot = scene->CreateObject();
    pivot.Hierarchy().Parent = parent;
    pivot.AddComponent<Transform>()->Position = position;
    pivot.AddComponent<Orderable>();
    return pivot;
}

GameObject Gui::CreateControl(GameObject parent) {
    GameObject control = CreatePivot(parent, 0);
    control.AddComponent<Mesh>();
    control.AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
    control.AddComponent<Touchable>();
    control.AddComponent<Colorable>();
    control.AddComponent<Sizeable>();
    control.AddComponent<Sprite>();
    control.AddReferenceComponent<class Atlas>(atlas);
    control.AddReferenceComponent<TextureComponent>(atlas);
    return control;
}

GameObject Gui::CreateControl(GameObject parent, const std::string& spriteName) {
    
    GameObject control = CreateControl(parent);
    control.GetComponent<Sprite>()->SpriteName = spriteName;
    
    return control;
}

GameObject Gui::CreateControl(GameObject parent, const std::string& spriteName, const Vector2 &position, const Vector2 &size) {
    
    GameObject control = CreateControl(parent, spriteName);
    
    control.GetComponent<Sizeable>()->Size = size;
    control.GetComponent<Transform>()->Position = position;
    
    return control;
}

GameObject Gui::CreateControl(GameObject parent, const std::string &spriteName, const Vector2& size) {
    GameObject control = CreateControl(parent);
    control.GetComponent<Sprite>()->SpriteName = spriteName;
    control.GetComponent<Sizeable>()->Size = size;
    return control;
}

GameObject Gui::CreateClipper(GameObject parent, bool push) {
    GameObject clipper = CreatePivot(parent, 0);
    clipper.AddReferenceComponent<Mesh>(parent);
    clipper.AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
    clipper.GetComponent<Renderable>()->Clip = push ? 1 : 2;
    clipper.AddReferenceComponent<Sizeable>(parent);
    clipper.AddReferenceComponent<Sprite>(parent);
    clipper.AddComponent<Touchable>();
    return clipper;
}

GameObject Gui::CreateFont(const std::string& fontFile) {
    GameObject font = scene->CreateObject();
    font.AddComponent<Font>()->LoadTTF(fontFile);
    font.GetComponent<Font>()->CharacterSetEverySize = 6;
    font.GetComponent<Font>()->maxTextureWidth = 1024;
    font.GetComponent<Font>()->maxTextureHeight = 1024;
    
    font.AddComponent<TextureComponent>();
    //font->SetID(fontFile);
    fonts.push_back(font);
    return font;
}

GameObject Gui::CreateLabel(GameObject parent, const Vector2 &position, const Vector2 &size, GameObject font, const std::string &text, float fontSize) {
    if (!font && !fonts.empty()) {
        font = fonts[0];
    }
    GameObject label = CreatePivot(parent, position);
    label.AddReferenceComponent<Font>(font);
    label.AddComponent<Mesh>();
    label.AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
    label.AddComponent<Colorable>();
    label.AddComponent<Sizeable>()->Size = size;
    label.AddReferenceComponent<TextureComponent>(font);
    label.AddComponent<Label>()->FontSize = fontSize;
    label.GetComponent<Label>()->Text = text;
    return label;
}

GameObject Gui::CreateLabelControl(GameObject parent, const std::string &spriteName, const Vector2 &position, const Vector2 &size, GameObject font, std::string text, float fontSize) {
    GameObject control = CreateControl(parent, spriteName, position, size);
    GameObject labelGO = CreateLabel(control, 0, size, font, text, fontSize);
    Label* label = labelGO.GetComponent<Label>();
    label->HAlignment = Font::HAlignment::Center;
    label->VAlignment = Font::VAlignment::Middle;
    //labelGO->ReplaceComponent<Sizeable>(control);
    
    return control;
}

GameObject Gui::CreateTextBox(GameObject parent, const std::string &spriteName, const Vector2 &position, const Vector2 &size, GameObject font, std::string text, float fontSize) {
    GameObject control = CreateControl(parent, spriteName, position, size);
    //GameObject labelGO = CreateLabel(control, 0, size, font, text, fontSize);
    
    if (!font && !fonts.empty()) {
        font = fonts[0];
    }

    GameObject labelGO = CreatePivot(control);
    labelGO.AddReferenceComponent<Font>(font);
    labelGO.AddComponent<Mesh>();
    labelGO.AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
    labelGO.AddComponent<Colorable>();
    labelGO.AddReferenceComponent<Sizeable>(control);
    labelGO.AddReferenceComponent<TextureComponent>(font);
    labelGO.AddComponent<Label>()->FontSize = fontSize;
    labelGO.GetComponent<Label>()->Text = text;

    Label* label = labelGO.GetComponent<Label>();
    label->HAlignment = Font::HAlignment::Center;
    label->VAlignment = Font::VAlignment::Middle;
    labelGO.AddComponent<TextBox>()->Text = text;
    labelGO.AddReferenceComponent<Touchable>(control);
    labelGO.GetComponent<Colorable>()->Color = Color::Black();
    return control;
}

GameObject Gui::CreateListbox(GameObject parent, const std::string &spriteName, const Vector2 &position, const Vector2 &size, GameObject& pivot) {
    GameObject listbox = CreateControl(parent, spriteName, position, size);
    listbox.AddComponent<Layouter>()->ChildrenLayoutMode = Layouter::LayoutMode::Vertical;
    listbox.GetComponent<Touchable>()->ClickThrough = false;
    listbox.AddComponent<Hoverable>();
    CreateClipper(listbox, true);
    GameObject p = CreatePivot(listbox);
    p.AddReferenceComponent<Sizeable>(listbox);
    p.AddReferenceComponent<Touchable>(listbox);
    //p.AddComponent<Draggable>()->Movement = Draggable::MovementMode::YAxis;
    //p.AddComponent<DraggableMotion>();
    //p.AddComponent<Velocity>()->MinimumSpeedBeforeStop = 5;
    //p.GetComponent<Velocity>()->Friction = 5;
    p.AddReferenceComponent<Hoverable>(listbox);
    p.AddComponent<ScrollWheelMover>()->Movement = {0,-100,0};
    p.GetComponent<ScrollWheelMover>()->Speed = 1.0f;
    
    //Limitable* limitable = p.AddComponent<Limitable>();
    //limitable->Size = p.GetComponent<Sizeable>();
    //limitable->View = listbox.GetComponent<Sizeable>();
    CreateClipper(listbox, false);
    pivot=p;
    return listbox;
}

const Gui::Fonts& Gui::GetFonts() const {
    return fonts;
}

GameObject Gui::CreateLayout(GameObject parent, const Vector2 &minSize, const Vector2 &desiredSize, const Vector2 &maxSize, Layouter::LayoutMode layoutMode) {
    GameObject control = CreatePivot(parent);
    control.AddComponent<Sizeable>();
    Layouter* l = control.AddComponent<Layouter>();
    l->Min = minSize;
    l->Desired = desiredSize;
    l->Max = maxSize;
    l->ChildrenLayoutMode = layoutMode;
    return control;
}

GameObject Gui::CreateLayoutControl(GameObject parent, const std::string &spriteName, const Vector2 &minSize, const Vector2 &desiredSize, const Vector2 &maxSize, Layouter::LayoutMode layoutMode) {
    GameObject control = CreateControl(parent, spriteName);
    Layouter* l = control.AddComponent<Layouter>();
    l->Min = minSize;
    l->Desired = desiredSize;
    l->Max = maxSize;
    l->ChildrenLayoutMode = layoutMode;
    return control;
}

void Gui::AddLayouter(GameObject object, const Vector2& minSize, const Vector2& desiredSize, const Vector2& maxSize, Layouter::LayoutMode layoutMode) {
    if (!object.GetComponent<Sizeable>()) {
        object.AddComponent<Sizeable>();
    }
    
    Layouter* l = object.AddComponent<Layouter>();
    l->Min = minSize;
    l->Desired = desiredSize;
    l->Max = maxSize;
    l->ChildrenLayoutMode = layoutMode;
}
