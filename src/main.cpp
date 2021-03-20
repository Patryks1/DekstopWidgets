#include "App.h"

int main() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	MyApp app;
	app.Run();

	return 0;
}
