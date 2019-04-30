#ifndef RAYMARCHJOB_H
#define RAYMARCHJOB_H

struct RayMarchJob
{
    unsigned int pixelNumber;
    unsigned int pixelsToRender;
    bool isShuttingDown;
    bool newJobAvailable;
};

#endif // RAYMARCHJOB_H
