
#include "Camera.hpp"
#include "Transform.hpp"
#include "Screen.hpp"
#include "MathHelper.hpp"

using namespace Mini;

Camera::Camera() {
	FieldOfView = 70;
	Near = 0.1f;
	Far = 512.0f;
    Orthographic = false;
    Mask = 0;
    Viewport = {0,0,1,1};
    Aspect = 0;
	
	Projection.Method = [this] (Matrix4x4& mat) {
        float fieldOfView = FieldOfView;
        if (!Orthographic()) {
            float aspect;
            if (MathHelper::FloatEqual(Aspect, 0, 0.001f)) {
                const Rect& viewport = Viewport;
                const Vector2& screenSize = Screen::MainScreen->Size;
                Rect screenRect = viewport * screenSize;
                aspect = screenRect.Aspect();
            } else {
                aspect = Aspect;
            }
            mat.InitPerspective(fieldOfView, aspect, Near, Far);
        } else {
            if (fieldOfView<=0.0001f) {
                const Rect& viewport = Viewport;
                const Vector2& screenSize = Screen::MainScreen->Size;
                Rect screenRect = viewport * screenSize;
                mat.InitOrthographic(0, screenRect.height, screenRect.width, 0, Near, Far);
            } else {
                const Rect& viewport = Viewport;
                const Vector2& screenSize = Screen::MainScreen->Size;
                Rect screenRect = viewport * screenSize;
                float aspect = screenRect.Aspect();
                
                float top = fieldOfView * 0.5f;
                float right = top * aspect;
                //(float left, float top, float right, float bottom, float nearValue, float farValue)
                mat.InitOrthographic(-right, top, right, -top, Near, Far);
            }
        }
    };
    
	ProjectionInverse.Method = [this] (Matrix4x4& mat) {
        mat = Projection().Invert();
    };
    
    auto changed = [this] () {
        Projection.MakeDirty();
        ProjectionInverse.MakeDirty();
    };

	FieldOfView.Changed.Bind(changed);
    Near.Changed.Bind(changed);
    Far.Changed.Bind(changed);
    Viewport.Changed.Bind(changed);
    Aspect.Changed.Bind([this] () {
        if (MathHelper::FloatEqual(Aspect, 0, 0.001f)) {
            Screen::MainScreen->Size.Changed.Bind(this, &Camera::ScreenSizeChanged);
        } else {
            Screen::MainScreen->Size.Changed.Unbind(this, &Camera::ScreenSizeChanged);
        }
    });
    
    FieldOfView.Changed.MarkDefaults();
    Near.Changed.MarkDefaults();
    Far.Changed.MarkDefaults();
    Viewport.Changed.MarkDefaults();
    Aspect.Changed.MarkDefaults();
    
    Screen::MainScreen->Size.Changed.Bind(this, &Camera::ScreenSizeChanged);
}

Camera::~Camera() {
    Screen::MainScreen->Size.Changed.Unbind(this, &Camera::ScreenSizeChanged);
}

void Camera::ScreenSizeChanged() {
    Projection.MakeDirty();
    ProjectionInverse.MakeDirty();
}

Matrix4x4 Camera::GetViewProjection(Transform* viewTransform) {
	return Projection().Multiply(viewTransform->WorldInverse);
}

Ray Camera::GetRay(Transform* viewTransform, const Vector2& screenPosition) {
    const Rect& viewPort = Viewport() * Screen::MainScreen->Size();
    
    Vector2 fromCenter = screenPosition - viewPort.Center();
    fromCenter /= (viewPort.Size() * 0.5f);
    
    Matrix4x4 viewProjection = GetViewProjection(viewTransform).Invert();
    
    Vector3 rayStartPosition = viewProjection.TransformPosition(Vector3(fromCenter.x,fromCenter.y,-1));
    Vector3 rayEndPosition = viewProjection.TransformPosition(Vector3(fromCenter.x,fromCenter.y,1));
    return Ray(rayStartPosition, rayEndPosition - rayStartPosition);
}

Vector3 Camera::TransformPointToViewSpace(Transform* viewTransform, const Vector3& worldPoint) {
    Matrix4x4 viewProjection = GetViewProjection(viewTransform);
    return viewProjection.TransformPosition(worldPoint);
}

Vector3 Camera::TransformPointToScreenSpace(Transform* viewTransform, const Vector3& worldPoint) {
    Vector2 screenPoint = TransformPointToViewSpace(viewTransform, worldPoint);
    const Rect& viewPort = Viewport() * Screen::MainScreen->Size();
    screenPoint *= (viewPort.Size() * 0.5f);
    screenPoint += viewPort.Center();
    return Vector3(screenPoint.x, screenPoint.y, worldPoint.z);
}

Vector3 Camera::TransformViewportToWorld(Transform* viewTransform, const Vector3& viewportPoint) {
    Matrix4x4 viewProjection = GetViewProjection(viewTransform).Invert();
    return viewProjection.TransformPosition(viewportPoint);
}

Vector3 Camera::TransformWorldToViewport(Transform* viewTransform, const Vector3& worldPoint) {
    Matrix4x4 viewProjection = GetViewProjection(viewTransform);
    return viewProjection.TransformPosition(worldPoint);
}

Vector3 Camera::TransformViewPositionToScreenSpace(Transform* viewTransform, const Vector3& viewPoint) {
    const Matrix4x4& viewProjection = Projection();
    Vector2 screenPoint = viewProjection.TransformPosition(viewPoint);
    const Rect& viewPort = Viewport() * Screen::MainScreen->Size();
    screenPoint *= (viewPort.Size() * 0.5f);
    screenPoint += viewPort.Center();
    return Vector3(screenPoint.x, screenPoint.y, viewPoint.z);
}
