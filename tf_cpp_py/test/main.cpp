#include "Model.h"

int main(int argc, const char * argv[])
{
    String message = "Saying";
    Model t = Model(message);
    t.say();
}
