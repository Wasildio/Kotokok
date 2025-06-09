#include "Controller.h"
#include <cstdlib>
#include <ctime>

int main() {
std::srand(static_cast<unsigned>(std::time(nullptr)));
Controller app(800, 800);
app.mainLoop();
return 0;
}
