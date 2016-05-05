void init_monitor(int, char *[]);
void reg_test();
void restart();
void ui_mainloop();
extern void init_device ();
extern void init_sdl ();

int main(int argc, char *argv[]) {
	/* Initialize the monitor. */
	init_monitor(argc, argv);

	/* Test the implementation of the ``CPU_state'' structure. */
	reg_test();

	/* Initialize the virtual computer system. */
	restart();

	/* Receive commands from user. */
    init_device();
    init_sdl();
	ui_mainloop();

	return 0;
}
