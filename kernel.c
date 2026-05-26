void kernel_main() {
    char *video = (char*)0xb8000;
    char *msg = "FroggyOS v0.1 Alpha!";
    
    for(int i = 0; msg[i]; i++) {
        video[i*2] = msg[i];
        video[i*2+1] = 0x0a;  // green on black
    }
    
    while(1);
}
