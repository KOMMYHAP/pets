#include <iostream>
// #include <SFML/Window.hpp>
// #include "SFML/System.hpp"
#include "EventDispatcherApplication.h"

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // Find your class. JNI_OnLoad is called from the correct class loader context for this to work.
    jclass c = env->FindClass("com/example/remotemouse/package/MyClass");
    if (c == nullptr) return JNI_ERR;

    // Register your class' native methods.
    static const JNINativeMethod methods[] = {
        {"nativeFoo", "()V", reinterpret_cast<void*>(nativeFoo)},
        {"nativeBar", "(Ljava/lang/String;I)Z", reinterpret_cast<void*>(nativeBar)},
    };
    int rc = env->RegisterNatives(c, methods, sizeof(methods)/sizeof(JNINativeMethod));
    if (rc != JNI_OK) return rc;

    return JNI_VERSION_1_6;
}

// int main(int argc, char **argv)
// {
// 	std::cout
// 		<< "Hello from preudo android application!" << std::endl
// 		<< "I'll be listen to system events: mouse moving and clicking and send it to another application." << std::endl;
//
// 	auto application = std::make_unique<EventDispatcherApplication>();
// 	application->ProcessCommandLine(argc, argv);
//
// 	sf::Window window(sf::VideoMode(600, 400), "Pseudo Android App");
// 	// window.setFramerateLimit(25);
//
// 	sf::Clock clock;
// 	sf::Time timeOnFrame = sf::milliseconds(15);
// 	sf::Time elapsedTimeOnFrame;
//
// 	bool shouldClose = false;
// 	while (not shouldClose)
// 	{
//
// 		sf::Event event;
// 		while (!shouldClose && window.pollEvent(event))
// 		{
// 			application->ProcessEvent(event);
// 			shouldClose = application->ShouldTerminate();
// 		}
//
// 		auto elapsedTimeInternal = TimeState::Milliseconds(elapsedTimeOnFrame.asMilliseconds());
// 		application->ProcessElapsedTime(elapsedTimeInternal);
//
// 		elapsedTimeOnFrame = clock.restart();
// 		if (elapsedTimeOnFrame < timeOnFrame)
// 		{
// 			sf::sleep(timeOnFrame - elapsedTimeOnFrame);
// 		}
// 	}
//
// 	window.close();
//
// 	return 0;
// }
