LOCAL_PATH := $(call my-dir) 
include $(CLEAR_VARS)
include E:/opencv/OpenCV-2.4.6-android-sdk-r2/OpenCV-2.4.6-android-sdk/sdk/native/jni/OpenCV.mk


LOCAL_C_INCLUDES += D:/Android/NDK/android-ndk-r9-windows-x86/android-ndk-r9/sources/cxx-stl/gnu-libstdc++/4.6/include
LOCAL_C_INCLUDES += D:/Android/NDK/android-ndk-r9-windows-x86/android-ndk-r9/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi/include





LOCAL_MODULE := pcvimg  

  
#1 �����������cpp�ļ��ĺ�  
define all-cpp-files-under  
$(patsubst ./%,%, $(shell find $(LOCAL_PATH) -name "*.cpp" -and -not -name ".*"))  
endef  
  
define all-subdir-cpp-files  
$(call all-cpp-files-under,.)  
endef  
  
#2 �����������c�ļ��ĺ꣬android��Ĭ�϶��壬�˴�������ʡ��  
define all-c-files-under  
$(patsubst ./%,%, $(shell find $(LOCAL_PATH) -name "*.c" -and -not -name ".*"))  
endef  
  
define all-subdir-c-files  
$(call all-c-files-under,.)  
endef  
  
#3 ͨ�����һ�ȡ���й����ļ��б�  
CPP_FILE_LIST := $(call all-subdir-cpp-files) 
C_FILE_LIST := $(call all-subdir-c-files)  
  
#4 ���빤���ļ���֮���Բ�ֱ�Ӽ�����Ҫ����һ��LOCAL_PATH���滻  
LOCAL_SRC_FILES := $(CPP_FILE_LIST:$(LOCAL_PATH)/%=%)   
LOCAL_SRC_FILES += $(C_FILE_LIST:$(LOCAL_PATH)/%=%) 


include $(BUILD_SHARED_LIBRARY)