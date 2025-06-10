void *hack_thread(void* ){
    ProcMap il2cppMap;
    do {
        il2cppMap = KittyMemory::getElfBaseMap("libil2cpp.so");
        sleep(1);
    } while (!il2cppMap.isValid() && mlovinit());
    setShader("unity_SHC");
    LogShaders();
    Wallhack();
    
    return NULL;
}