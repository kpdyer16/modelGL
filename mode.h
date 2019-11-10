#ifndef MODE_H
#define MODE_H

struct MModes
{
    enum Modes
    {
        Neutral, // safe mode. no actions associated
        Select, // can add triangles here with mouse
        Translate, // moves objects only
        Rotate,    // rotates objects only
        Scale,     // scales objects only
        CameraTranslate, // moves eye position
        TargetTranslate, // moves center position
        RotateCameraZ,   // rotates up vector
        NearPlane,       // adjust near plane
        FarPlane         // adjust far plane
    };
};

struct KModes
{
    enum Modes
    {
        Neutral, // safe mode. no actions associated
        Object, // actions only work on the currently selected object
        CameraTranslate, // actions only work on the camera
        TargetTranslate, // actions only work on the target
        CameraZ, // rotate cameraZ
        NearPlane,
        FarPlane
    };

};

#endif // MODE_H
