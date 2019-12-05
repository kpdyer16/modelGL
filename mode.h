#ifndef MODE_H
#define MODE_H

struct Mode
{
    enum mode
    {
        triangles,
        selection,
        translate,
        rotate,
        scale,
        translateCam,
        translateTarget,
        rotateCameraZ,
        nearclip,
        farclip
    };
};

#endif // MODE_H
