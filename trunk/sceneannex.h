#ifndef SCENEANNEX_H_
#define SCENEANNEX_H_

class SceneAnnex
{
public:
	SceneAnnex(){}
    virtual ~SceneAnnex(){}

    Vec m_eye;
    Vec m_lightColor;
    Vec m_lightPosition;
    Vec Ka, Kd, Ks;    // Ambient, Diffuse, Specular constants
    Vec shineReflectOpacity;
};

#endif /*SCENEANNEX_H_*/
