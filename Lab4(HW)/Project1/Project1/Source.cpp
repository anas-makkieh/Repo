#include<GL/glew.h>
#include <iostream>

// 1. التضمين:
// تعريف GLEW_STATIC يخبر المعالج المسبق أننا نستخدم مكتبة ساكنة (تدمج في الـ exe)
// يجب دائماً تضمين GLEW قبل GLFW لأنها تحتوي على تعريفات OpenGL الضرورية
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

int main()
{
    // 2. تهيئة GLFW:
    // هذه الدالة تجهز نظام التشغيل لاستقبال طلبات النوافذ
    glfwInit();

    // 3. ضبط خصائص النافذة (Window Hints):
    // قبل إنشاء النافذة، نرسل "تلميحات" للإعدادات التي نريدها

    // نطلب نسخة OpenGL 3.3 (الرقم الأول Major والرقم الثاني Minor)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // نحدد أننا نريد "Core Profile"
    // هذا يعني: "لا تسمح لي باستخدام الدوال القديمة (Deprecated)".
    // هذا يجبرنا على تعلم الطرق الحديثة فقط.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // سطر ضروري لمستخدمي Mac ليتم تفعيل الكود
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // نمنع المستخدم من تغيير حجم النافذة لتبسيط الأمور حالياً
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // 4. إنشاء النافذة الفعلية:
    // الدالة تعيد مؤشراً (Pointer) للنافذة. إذا فشلت، تعيد nullptr.
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);

    // التعامل مع كثافة الشاشة (Pixel Density):
    // في الشاشات الحديثة، حجم النافذة المنطقي يختلف عن عدد البكسلات الفعلي.
    // هذه الدالة تعطينا الأبعاد الحقيقية للبكسلات لاستخدامها في الرسم.
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate(); // تنظيف الموارد قبل الخروج
        return EXIT_FAILURE;
    }

    // تفعيل الـ Context:
    // نخبر GLFW أن تجعل هذه النافذة هي "مكان الرسم الحالي" لـ OpenGL
    glfwMakeContextCurrent(window);

    // 5. تهيئة GLEW:
    // glewExperimental = GL_TRUE يضمن أن GLEW تستخدم تقنيات حديثة للكشف عن الدوال
    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // تحديد منفذ العرض (Viewport):
    // نخبر OpenGL أن مساحة الرسم تبدأ من (0,0) وتمتد لعرض وارتفاع الشاشة الفعليين
    glViewport(0, 0, screenWidth, screenHeight);

    // 6. حلقة العرض (Game Loop):
    // هذه الحلقة تستمر بالدوران طالما لم يطلب المستخدم إغلاق النافذة
    while (!glfwWindowShouldClose(window))
    {
        // التحقق من الأحداث (Events):
        // هل ضغط المستخدم زر؟ هل حرك الماوس؟ GLFW تلتقط هذه الأحداث هنا.
        glfwPollEvents();

        // --- مرحلة الرسم ---

        // تحديد لون المسح: دالة "حالة" تحدد اللون الذي سنستخدمه لمسح الشاشة (لون تركوازي)
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // تنفيذ المسح: دالة "أمر" تستخدم اللون المحدد أعلاه لمسح مخزن الألوان (Color Buffer)
        glClear(GL_COLOR_BUFFER_BIT);

        // التبديل (Swap Buffers):
        // بما أننا رسمنا في المخزن الخلفي، نقوم الآن بتبديله ليظهر للمستخدم

        glfwSwapBuffers(window);
    }

    // 7. الإنهاء:
    // تنظيف جميع موارد GLFW وحذف النافذة من الذاكرة
    glfwTerminate();

    return EXIT_SUCCESS;
}

