set(arora_SRCS ${arora_SRCS}
    cookiejar/cookiedialog.cpp
    cookiejar/cookieexceptionsmodel.cpp
    cookiejar/cookiemodel.cpp
    cookiejar/cookieexceptionsdialog.cpp
    cookiejar/cookiejar.cpp)

set(arora_FORMS ${arora_FORMS}
    cookiejar/cookies.ui
    cookiejar/cookiesexceptions.ui)

include(cookiejar/networkcookiejar/src.cmake REQUIRED)
