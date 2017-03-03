#pragma once

#include "../SDK/SDK.h"

namespace Math {
    void inline SinCos(float radians, float *sine, float *cosine);
    void AngleVectors(const QAngle &angles, Vector &forward);
    void NormalizeAngles(QAngle& angle);
    void ClampAngles(QAngle& angle);
    QAngle DeltaAngles(const QAngle& start, const QAngle& end);
    void CorrectMovement(QAngle vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove);
    float GetDistance(const Vector& source, const Vector& destination);
    float Norm(const Vector& vec);
    Vector ScaleVectorTo(const Vector& vec, float norm);
    float GetFov(const QAngle &viewAngle, const QAngle &aimAngle);
    void VectorAngles(const Vector &forward, QAngle &angles);
    QAngle CalcAngle(Vector src, Vector dst);
}
