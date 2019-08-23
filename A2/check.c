int main() {
    int c;
    while (1) {
        set_mode(1);
        while (!(c = get_key()))
            usleep(10000);
        printf("key %d\n", c);
    }
}