//
//  ZsBuiltinParams.h
//  Pods
//
//  Created by zhaowanfei on 2023/5/9.
//

#ifndef ZsShader_h
#define ZsShader_h

#define ZS_CORE_EXPORT

#define Vector4 float
#define Matrix4 float
#define Texture float


#include <vector>

namespace zs {


enum BuiltinParamSlot
{
    PS_ShaderVecLight0Diffuse,
    PS_ShaderVecCount,
    
    PS_ShaderMatProj ,
    PS_ShaderMatView,
    PS_ShaderMatInvView,
    PS_ShaderMatCount,
    
    
    PS_ShaderTexEnvWhite ,
    PS_ShaderTexEnvBlack,
    PS_ShaderTexEnvRed,
    PS_ShaderTexCount
};



enum BuiltinParamType{
    PT_VEC4,
    PT_MAT4,
    PT_TEX2D,
    PT_COUNT,
};



class ZS_CORE_EXPORT BuiltinParams
{
public:
    struct BuilitParamVal
    {
        const void* data;
        BuiltinParamType type;
        BuiltinParamSlot slot;
    };
private:

    std::vector<Vector4> m_Vec4ParamValues;
    std::vector<Matrix4> m_Mat4ParamValues;
    std::vector<Texture> m_Tex2DParamValues;
    std::vector<BuiltinParams::BuilitParamVal> m_Params;


public:
    //static BuiltinParams& Instance();
    BuiltinParams();

    
private:
    void AddParam(BuiltinParamType type, BuiltinParamSlot slot);
};



}// namespace zs





#endif /* ZsBuiltinParams_h */
