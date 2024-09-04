#include "skippy_private.h"
// include the Defold SDK
#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_ANDROID)

bool Started = false;

static JNIEnv *Attach()
{
    JNIEnv *env;
    JavaVM *vm = dmGraphics::GetNativeAndroidJavaVM();
    vm->AttachCurrentThread(&env, NULL);
    return env;
}

static bool Detach(JNIEnv *env)
{
    bool exception = (bool)env->ExceptionCheck();
    env->ExceptionClear();
    JavaVM *vm = dmGraphics::GetNativeAndroidJavaVM();
    vm->DetachCurrentThread();
    return !exception;
}

namespace
{
    struct AttachScope
    {
        JNIEnv *m_Env;
        AttachScope() : m_Env(Attach())
        {
        }
        ~AttachScope()
        {
            Detach(m_Env);
        }
    };
}

static jclass GetClass(JNIEnv *env, const char *classname)
{
    jclass activity_class = env->FindClass("android/app/NativeActivity");
    jmethodID get_class_loader = env->GetMethodID(activity_class, "getClassLoader", "()Ljava/lang/ClassLoader;");
    jobject cls = env->CallObjectMethod(dmGraphics::GetNativeAndroidActivity(), get_class_loader);
    jclass class_loader = env->FindClass("java/lang/ClassLoader");
    jmethodID find_class = env->GetMethodID(class_loader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

    jstring str_class_name = env->NewStringUTF(classname);
    jclass outcls = (jclass)env->CallObjectMethod(cls, find_class, str_class_name);
    env->DeleteLocalRef(str_class_name);
    return outcls;
}

int Skippy_PlatformStart()
{
    // Do nothing if already running
    if (Started)
    {
        return 1;
    }

    // prepare JNI
    AttachScope attachscope;
    JNIEnv *env = attachscope.m_Env;
    jclass cls = GetClass(env, "com.naakkadev.skippy.SkippyExtension");

    // call method
    jmethodID start_step = env->GetStaticMethodID(cls, "startStep", "(Landroid/content/Context;)V");
    env->CallStaticVoidMethod(cls, start_step, dmGraphics::GetNativeAndroidActivity());

    Started = true;

    return 1;
}

int Skippy_PlatformStop()
{
    // Do nothing if not running
    if (!Started)
    {
        return 1;
    }

    // prepare JNI
    AttachScope attachscope;
    JNIEnv *env = attachscope.m_Env;
    jclass cls = GetClass(env, "com.naakkadev.skippy.SkippyExtension");

    // call method
    jmethodID stop_step = env->GetStaticMethodID(cls, "stopStep", "(Landroid/content/Context;)V");
    env->CallStaticVoidMethod(cls, stop_step, dmGraphics::GetNativeAndroidActivity());

    Started = false;

    return 1;
}

int Skippy_GetSteps()
{
    // prepare JNI
    AttachScope attachscope;
    JNIEnv *env = attachscope.m_Env;
    jclass cls = GetClass(env, "com.naakkadev.skippy.SkippyExtension");

    // call method
    jmethodID get_steps = env->GetStaticMethodID(cls, "getSteps", "()I");
    jint step_count = env->CallStaticIntMethod(cls, get_steps);

    return step_count;
}

int Skippy_GetStepDetector()
{
    // prepare JNI
    AttachScope attachscope;
    JNIEnv *env = attachscope.m_Env;
    jclass cls = GetClass(env, "com.naakkadev.skippy.SkippyExtension");

    // call method
    jmethodID get_stepdetector = env->GetStaticMethodID(cls, "getStepDetector", "()I");
    jint step_count = env->CallStaticIntMethod(cls, get_stepdetector);

    return step_count;
}

#endif // DM_PLATFORM_ANDROID