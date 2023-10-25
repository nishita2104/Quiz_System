
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// int *response;

struct CDMA
{
    int **wtable;
    int **copy;
    int *response;
    int *channel_sequence;
};

void setUp(struct CDMA *channel, int num_stations);
void listenTo(struct CDMA *channel, int sourceStation, int num_stations);
int buildWalshTable(struct CDMA *channel, int len, int i1, int i2, int j1, int j2, int isBar);
void showWalshTable(struct CDMA *channel, int num_stations);

// int main() {
//     struct CDMA channel;
//     setUp(&channel, 2);
//     int data0[2] = {0,1};
//     int data1[2] = {1,0};
//     // Encode(channel, data0, 2, 0);    
//     // Encode(channel, data1, 2, 1); 
// for (int i = 0; i < 4; i++)
// {
//     channel.response[i]=0;
// }

// Encode2(&channel,data0,2,0);
// Encode2(&channel,data1,2,1); 
// printf("op\n");
// for (int i = 0; i < 4; i++)
// {
//     printf("%d ",channel.response[i]);
// }
// return 0;
// }

void setUp(struct CDMA *channel, int num_stations)
{
    channel->wtable = (int **)malloc(num_stations * sizeof(int *));
    channel->copy = (int **)malloc(num_stations * sizeof(int *));
    channel->response = (int*)malloc(num_stations *2* sizeof(int));

    for (int i = 0; i < num_stations; i++)
    {
        channel->wtable[i] = (int *)malloc(num_stations * sizeof(int));
        channel->copy[i] = (int *)malloc(num_stations * sizeof(int));
    }

    buildWalshTable(channel, num_stations, 0, num_stations - 1, 0, num_stations - 1, 0);

    showWalshTable(channel, num_stations);
    printf("setup done \n");
}

// int Encode(struct CDMA *channel, int *data, int num_stations, int id)
// {  
//     for (int k = 0; k <2 ; k++)
//     {
//         for (int j = 0; j < num_stations; j++)
//         {
//             channel->copy[id][j] = channel->wtable[id][j];
//             channel->wtable[id][j] *= data[k];
//         }

//         for (int j = 0; j < num_stations; j++)
//         {
//             channel->response[k * num_stations + j] += channel->wtable[id][j];
//         }
//     }
   
//     printf("gooooooodddd");
//     return 0;
// }

void Encode2(struct CDMA *channel, int *data, int num_stations, int id)
    {
        for (int j = 0; j < 2; j++)
            {
                for (int i = 0; i < num_stations; i++)
                {
            channel->response[j*num_stations+i]+=channel->wtable[id][i]*data[j];
        }
        
    }
    for (int i = 0; i < 4; i++)
{
    // printf("%d ",channel->response[i]);
}
}

void listenTo(struct CDMA *channel, int sourceStation, int num_stations)
{
    int innerProduct = 0;

    for (int i = 0; i < num_stations; i++)
    {
        innerProduct += channel->copy[sourceStation][i] * channel->channel_sequence[i];
    }

    printf("The data received is: %d\n", innerProduct / num_stations);
}

int buildWalshTable(struct CDMA *channel, int len, int i1, int i2, int j1, int j2, int isBar)
{
    if (len == 2)
    {
        if (!isBar)
        {
            channel->wtable[i1][j1] = 1;
            channel->wtable[i1][j2] = 1;
            channel->wtable[i2][j1] = 1;
            channel->wtable[i2][j2] = -1;
        }
        else
        {
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

void showWalshTable(struct CDMA *channel, int num_stations)
{
    printf("\n");

    for (int i = 0; i < num_stations; i++)
    {
        for (int j = 0; j < num_stations; j++)
        {
            printf("%d ", channel->wtable[i][j]);
        }
        printf("\n");
    }
    printf("-------------------------\n\n");
    for (int i = 0; i < num_stations; i++)
    {
        for (int j = 0; j < num_stations; j++)
        {
            printf("%d ", channel->copy[i][j]);
        }
        printf("\n");
    }
    printf("-------------------------\n\n");
}