LOCAL_PATH := $(call my-dir) 
include $(CLEAR_VARS)
include E:/opencv/OpenCV-2.4.6-android-sdk-r2/OpenCV-2.4.6-android-sdk/sdk/native/jni/OpenCV.mk


LOCAL_C_INCLUDES += D:/Android/NDK/android-ndk-r9-windows-x86/android-ndk-r9/sources/cxx-stl/gnu-libstdc++/4.6/include
LOCAL_C_INCLUDES += D:/Android/NDK/android-ndk-r9-windows-x86/android-ndk-r9/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi/include





LOCAL_MODULE := pcvimg  

  
#1 定义查找所有cpp文件的宏  
define all-cpp-files-under  
$(patsubst ./%,%, $(shell find $(LOCAL_PATH) -name "*.cpp" -and -not -name ".*"))  
endef  
  
define all-subdir-cpp-files  
$(call all-cpp-files-under,.)  
endef  
  
#2 定义查找所有c文件的宏，android有默认定义，此处可酌情省略  
define all-c-files-under  
$(patsubst ./%,%, $(shell find $(LOCAL_PATH) -name "*.c" -and -not -name ".*"))  
endef  
  
define all-subdir-c-files  
$(call all-c-files-under,.)  
endef  
  
#3 通过查找获取所有工程文件列表  
CPP_FILE_LIST := $(call all-subdir-cpp-files) 
C_FILE_LIST := $(call all-subdir-c-files)  
  
#4 加入工程文件，之所以不直接加是需要进行一个LOCAL_PATH的替换  
LOCAL_SRC_FILES := $(CPP_FILE_LIST:$(LOCAL_PATH)/%=%)   
LOCAL_SRC_FILES += $(C_FILE_LIST:$(LOCAL_PATH)/%=%) 


include $(BUILD_SHARED_LIBRARY)