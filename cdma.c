#include <stdio.h>
#include <stdlib.h>

struct CDMA {
    int** wtable;
    int** copy;
    int* channel_sequence;
};

void setUp(struct CDMA* channel, int* data, int num_stations);
void listenTo(struct CDMA* channel, int sourceStation, int num_stations);
int buildWalshTable(struct CDMA* channel, int len, int i1, int i2, int j1, int j2, int isBar);
void showWalshTable(struct CDMA* channel, int num_stations);

// int main() {
//     int num_stations = 8;
//     int data[] = {-1, -1, 0, 1,0,0,0,0};
//     struct CDMA channel;

//     setUp(&channel, data, num_stations);

//     int sourceStation = 2;
//     listenTo(&channel, sourceStation, num_stations);

//     return 0;
// }

void setUp(struct CDMA* channel, int* data, int num_stations) {
    channel->wtable = (int**)malloc(num_stations * sizeof(int*));
    channel->copy = (int**)malloc(num_stations * sizeof(int*));

    for (int i = 0; i < num_stations; i++) {
        channel->wtable[i] = (int*)malloc(num_stations * sizeof(int));
        channel->copy[i] = (int*)malloc(num_stations * sizeof(int));
    }

    buildWalshTable(channel, num_stations, 0, num_stations - 1, 0, num_stations - 1, 0);

    showWalshTable(channel, num_stations);

    channel->channel_sequence = (int*)malloc(num_stations * sizeof(int));

    for (int i = 0; i < num_stations; i++) {
        for (int j = 0; j < num_stations; j++) {
            channel->copy[i][j] = channel->wtable[i][j];
            channel->wtable[i][j] *= data[i];
        }
    }

    for (int i = 0; i < num_stations; i++) {
        for (int j = 0; j < num_stations; j++) {
            channel->channel_sequence[i] += channel->wtable[j][i];
        }
    }
}

void listenTo(struct CDMA* channel, int sourceStation, int num_stations) {
    int innerProduct = 0;

    for (int i = 0; i < num_stations; i++) {
        innerProduct += channel->copy[sourceStation][i] * channel->channel_sequence[i];
    }

    printf("The data received is: %d\n", innerProduct / num_stations);
}

int buildWalshTable(struct CDMA* channel, int len, int i1, int i2, int j1, int j2, int isBar) {
    if (len == 2) {
        if (!isBar) {
            channel->wtable[i1][j1] = 1;
            channel->wtable[i1][j2] = 1;
            channel->wtable[i2][j1] = 1;
            channel->wtable[i2][j2] = -1;
        }
        else {
            channel->wtable[i1][j1] = -1;
            channel->wtable[i1][j2] = -1;
            channel->wtable[i2][j1] = -1;
            channel->wtable[i2][j2] = 1;
        }
        return 0;
    }

    int midi = (i1 + i2) / 2;
    int midj = (j1 + j2) / 2;

    buildWalshTable(channel, len / 2, i1, midi, j1, midj, isBar);
    buildWalshTable(channel, len / 2, i1, midi, midj + 1, j2, isBar);
    buildWalshTable(channel, len / 2, midi + 1, i2, j1, midj, isBar);
    buildWalshTable(channel, len / 2, midi + 1, i2, midj + 1, j2, !isBar);

    return 0;
}

void showWalshTable(struct CDMA* channel, int num_stations) {
    printf("\n");

    for (int i = 0; i < num_stations; i++) {
        for (int j = 0; j < num_stations; j++) {
            printf("%d ", channel->wtable[i][j]);
        }
        printf("\n");
    }
    printf("-------------------------\n\n");
    for (int i = 0; i < num_stations; i++) {
        for (int j = 0; j < num_stations; j++) {
            printf("%d ", channel->copy[i][j]);
        }
        printf("\n");
    }
    printf("-------------------------\n\n");
}
