

class BlockBingoField {
    char blocks[16];
    int number;
}

class BlockInfo {
    int color;
    int x;
    int y;
}

#define NULL_BLOCK -1
#define POWER_BLOCK 0
#define RED_BLOCK 1
#define YELLOW_BLOCK 2
#define GREEN_BLOCK 3
#define BLUE_BLOCK 4

class PathNode {
    int contains[4];
    // destination;
    int number;
    PathNode *connection[8];
    bool isPathed;

    PathNode() {
        for(int i = 0; i < 4; i++) {
            contains[i] = NULL_BLOCK;
        }
        number = -1;
        for(int i = 0; i < 8; i++) {
            connection[i] = NULL;
        }
        isPathed = false;
    }
}

PathNode *pathNode;

void TableCreate(BlockInfo *blockInfo) {
    pathNode = new PathNode[4 * 4 + 4 * 3 * 2];
    int n = 0;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(i + j % 2 == 0)pathNode[i + j * 4].contains[0] = blockInfo[n];
        }
    }
}