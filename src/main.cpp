#include <cstdio>
#include <cstring>

using namespace std;

void train();
void test();
void resume();

static const char help[] = "Joint Cascade Face Detection and Alignment\n\n"
                           "train:  train JDA classifier and regressor for face detection\n"
                           "        and face alignemnt\n"
                           "test:   test the model trained by command `train`\n"
                           "resume: resume a previous training status\n";

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf(help);
    }
    else if (strcmp(argv[1], "train") == 0) {
        train();
    }
    else if (strcmp(argv[1], "test") == 0) {
        test();
    }
    else if (strcmp(argv[1], "resume") == 0) {
        resume();
    }
    else {
        printf(help);
    }
    return 0;
}
