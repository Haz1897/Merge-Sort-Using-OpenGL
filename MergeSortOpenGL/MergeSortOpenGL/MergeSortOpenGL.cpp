#include<GL/glut.h>
#include <stdio.h>
#include <time.h>
//This struct was made to make drawing squares around a number easier.
struct Number{
    int value;
    int squareVertices [4][2];// squareVertices[n][0]= x(n) , squareVertices[n][1] = y(n) . Ex: squareVertices[0][0] = x(0) , squareVertices[0][1] = y(0)
};
//This struct was made to store arrays along with their length, branches and Numbers.
struct Array{
    int *values;
    int length;
    bool isLeaf,isSorted;
    Array *left,*right;
    Number *numbers;
};

//Used to create a subarray of an array.
Array subArray(Array arr,int start, int end) {
    Array temp;
    temp.values = new int[end - start];
    temp.length = end - start;
    for (int i = 0;i < temp.length;i++) {
        temp.values[i] = arr.values[start+i];
    }
    return temp;
}
//Constructor to give default values to array.
Array Construct() {
    Array temp;
    temp.isLeaf = false;
    temp.isSorted = false;
    temp.left = NULL;
    temp.right = NULL;
    temp.numbers = NULL;
    return temp;
}

Array *startingArrays[200],sortingArrays[200]; //Used to store each step.
Array startingArray,sortedArray;//Unaltered array to store original numbers and Sorted array used to store sorting results.
int startingArrayLength = 0,sortingPointer=0; //Used to store arrays in the jagged array "arrays".
int nextIndex = 1; //Used to indicate the next array in line.
int size = 200; //Variable used to dynamically adjust cell size. (default 200)
int Current = nextIndex; //Indicates the current array (highlighted blue)
bool isGoingBack=false,isDone=false; //Used for nextStep() logic.

void Merge(Array arr, Array leftSubArray, Array rightSubArray) {
    int i = 0, l = 0, r = 0; //Pointers used to traverse arr, leftSubArray and rightSubarray respectively;
    while (l < leftSubArray.length && r < rightSubArray.length)
    {
        //Check if leftSubArray has a lower value.
        if (leftSubArray.values[l] < rightSubArray.values[r])
        {
            arr.values[i] = leftSubArray.values[l];
            i++;
            l++;
        }
        //Else, rightSubArray has a lower value.
        else
        {
            arr.values[i] = rightSubArray.values[r];
            i++;
            r++;
        }
    }
    //Used to empty leftSubArray in case anything was left.
    while (l < leftSubArray.length)
    {
        arr.values[i] = leftSubArray.values[l];
        i++;
        l++;
    }
    //Used to empty rightSubArray in case anything was left.
    while (r < rightSubArray.length)
    {
        arr.values[i] = rightSubArray.values[r];
        i++;
        r++;
    }
}
void MergeSort(Array arr) {
    if (arr.length == 1){ //Base case: check if length of array = 1;
        return;
}
    int mid = arr.length / 2;
    Array leftSubArray =subArray(arr,0, mid); //Create left sub array starting from arr[0] and ending at index arr[mid];
    Array rightSubArray = subArray(arr,mid, arr.length);    //Create left sub array starting from arr[0] and ending at index arr[length - mid];
    MergeSort(leftSubArray);
    MergeSort(rightSubArray);
    Merge(arr, leftSubArray, rightSubArray);
    sortingArrays[sortingPointer++] = arr;

}

void drawNumber(int number, int startX, int startY) {
    int remainder,temp=number,digits=0;
    bool isNegative = false;

    //Check if number is negative.
    if (number < 0) {
        number *= -1;
        isNegative = true;
    }
    //Get number of digits;
    do {
        temp = (int)(temp / 10);
        digits++;
    } while (temp != 0);
    if (isNegative)
        digits++;

    int numberPadding = size/20;  //Used to keep numbers apart;
    int totalNumberSize = (size * 18) / 20;
    int numberSize;
    if (digits > 3)
        numberSize = totalNumberSize / digits;
    else
        numberSize = (int)(size * 6 / 20); //Represents length/height of every number;
    int numberX = (int)(startX + size - (size / 20)-(numberPadding/3)); //Starting X of each digit;
    int numberY = (int)(startY - (size * 7 / 20)); //Starting Y of each digit;
    
    //If number is 1 digit, shift it to middle.
    if (digits == 1)
        numberX -= numberSize;
    else if (digits == 2)
        numberX -= numberSize / 2;
    
    do {
        remainder = number % 10;
        number = number / 10;
        switch (remainder) {
        case 0:
            glBegin(GL_LINE_LOOP);
            glVertex2f(numberX-numberSize+(numberPadding*1.3),numberY);
            glVertex2f(numberX,numberY);
            glVertex2f(numberX,numberY-numberSize);
            glVertex2f(numberX-numberSize + (numberPadding * 1.3),numberY-numberSize);
            glEnd();
            break;
        case 1:
            glBegin(GL_LINES);
            glVertex2f(numberX-(numberSize/3), numberY);
            glVertex2f(numberX - (numberSize / 3),numberY-numberSize);
            glEnd();
            break;
        case 2:
            glBegin(GL_LINE_STRIP);
            glVertex2f(numberX-numberSize + (numberPadding * 1.3),numberY);
            glVertex2f(numberX,numberY);
            glVertex2f(numberX,numberY - (numberSize / 2));
            glVertex2f(numberX-numberSize + (numberPadding * 1.3),numberY-(numberSize/2));
            glVertex2f(numberX-numberSize + (numberPadding * 1.3),numberY-numberSize);
            glVertex2f(numberX,numberY-numberSize);
            glEnd();
            break;
        case 3:
            glBegin(GL_LINE_STRIP);
            glVertex2f(numberX - numberSize +(numberPadding * 1.3), numberY);
            glVertex2f(numberX, numberY);
            glVertex2f(numberX, numberY - numberSize);
            glVertex2f(numberX - numberSize + (numberPadding * 1.3), numberY - numberSize);
            glEnd();
            glBegin(GL_LINES);
            glVertex2f(numberX,numberY-(numberSize/2));
            glVertex2f(numberX-numberSize + (numberPadding * 1.3),numberY-(numberSize/2));
            glEnd();
            break;
        case 4:
            glBegin(GL_LINE_STRIP);
            glVertex2f(numberX-numberSize+(numberPadding*1.3),numberY);
            glVertex2f(numberX - numberSize + (numberPadding*1.3), numberY-(numberSize/2));
            glVertex2f(numberX, numberY - (numberSize / 2));
            glVertex2f(numberX,numberY);
            glVertex2f(numberX,numberY-numberSize);
            glEnd();
            break;
        case 5:
            glBegin(GL_LINE_STRIP);
            glVertex2f(numberX, numberY);
            glVertex2f(numberX - numberSize + (numberPadding*1.3), numberY);
            glVertex2f(numberX - numberSize + (numberPadding*1.3), numberY - (numberSize / 2));
            glVertex2f(numberX, numberY - (numberSize / 2));
            glVertex2f(numberX, numberY - numberSize);
            glVertex2f(numberX - numberSize + (numberPadding*1.3), numberY - numberSize);
            glEnd();
            break;
        case 6:
            glBegin(GL_LINE_STRIP);
            glVertex2f(numberX,numberY);
            glVertex2f(numberX - numberSize + (numberPadding * 1.3), numberY);
            glVertex2f(numberX - numberSize + (numberPadding * 1.3), numberY-numberSize);
            glVertex2f(numberX , numberY-numberSize);
            glVertex2f(numberX , numberY-(numberSize/2));
            glVertex2f(numberX - numberSize + (numberPadding * 1.3), numberY - (numberSize / 2));
            glEnd();
            break;
        case 7:
            glBegin(GL_LINE_STRIP);
            glVertex2f(numberX-numberSize+(numberPadding*1.3),numberY);
            glVertex2f(numberX,numberY);
            glVertex2f(numberX,numberY-numberSize);
            glEnd();
            break;
        case 8:
            glBegin(GL_LINE_LOOP);
            glVertex2f(numberX - numberSize + (numberPadding * 1.3), numberY);
            glVertex2f(numberX, numberY);
            glVertex2f(numberX, numberY - numberSize);
            glVertex2f(numberX - numberSize + (numberPadding * 1.3), numberY - numberSize);
            glEnd();
            glBegin(GL_LINES);
            glVertex2f(numberX, numberY - (numberSize / 2));
            glVertex2f(numberX - numberSize + (numberPadding * 1.3), numberY - (numberSize / 2));
            glEnd();
            break;
        case 9:
            glBegin(GL_LINE_LOOP);
            glVertex2f(numberX,numberY-(numberSize/2));
            glVertex2f(numberX-numberSize+(numberPadding*1.3), numberY - (numberSize / 2));
            glVertex2f(numberX-numberSize+(numberPadding*1.3), numberY);
            glVertex2f(numberX, numberY);
            glVertex2f(numberX, numberY-numberSize);
            glEnd();
            break;
        }
        numberX -= numberSize;
    } while (number != 0);
    if (isNegative) {
        glBegin(GL_LINES);
        glVertex2f(numberX,numberY-(numberSize/2));
        glVertex2f(numberX-numberSize+(numberPadding*1.3), numberY - (numberSize / 2));
        glEnd();

    }
}

//Function used to draw squares.
void drawSquare(Array arr,int index) {
    glLoadIdentity();
    glOrtho(-1920, 1920, -1080, 1080,-100,100);
    for (int i = 0;i < arr.length;i++) {
        Number number = arr.numbers[i];
        if (index == Current && nextIndex != 1)
            glColor3f(0.5,1,1);
        else 
            glColor3f(1, 1, 1);
        glLineWidth(1);
        glBegin(GL_QUADS);
        for (int i = 0;i < 4;i++) {
            glVertex2f(number.squareVertices[i][0], number.squareVertices[i][1]);
        }
        glEnd();
        glColor3f(0, 0, 0);
        glLineWidth(3);
        glBegin(GL_LINE_LOOP);
        for (int i = 0;i < 4;i++) {
            glVertex2f(number.squareVertices[i][0], number.squareVertices[i][1]);
        }
        glEnd();
        drawNumber(arr.values[i],number.squareVertices[0][0],number.squareVertices[0][1]);
    }
}
//Goes through all present arrays to draw unsorted arrays.
void drawArray(Array *arrays[]) {

    for (int i = 0;i < nextIndex;i++) {
        if(!arrays[i]->isSorted)
            drawSquare(*arrays[i],i);
    }

}
void display() {
    glClearColor(0.25, 0.25, 0.3, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawArray(startingArrays);
    glFlush();
}

//This method is used to give vertices to the array
void giveCoordinates(Array *arr,int startX,int startY) {
    arr->numbers = new Number[arr->length];//Initialize numbers array which holds coordinates;
   
    Array *temp;
    //First cell setup.
    arr->numbers[0].squareVertices[0][0] = startX               ;arr->numbers[0].squareVertices[0][1] = startY;     //(x1,y1)
    arr->numbers[0].squareVertices[1][0] = startX+size          ;arr->numbers[0].squareVertices[1][1] = startY;     //(x2,y2)
    arr->numbers[0].squareVertices[2][0] = startX+size          ;arr->numbers[0].squareVertices[2][1] = startY-size;//(x3,y3)
    arr->numbers[0].squareVertices[3][0] = startX               ;arr->numbers[0].squareVertices[3][1] = startY-size;//(x4,y4)
   
    //Set-up all remaining cells using the first as a reference;
    for (int i = 1;i < arr->length;i++) {
        arr->numbers[i].squareVertices[0][0] = arr->numbers[i-1].squareVertices[1][0]               ;arr->numbers[i].squareVertices[0][1] = arr->numbers[i - 1].squareVertices[1][1];//(x1,y1)
        arr->numbers[i].squareVertices[1][0] = arr->numbers[i-1].squareVertices[1][0] + size        ;arr->numbers[i].squareVertices[1][1] = arr->numbers[i - 1].squareVertices[1][1];//(x2,y2)
        arr->numbers[i].squareVertices[2][0] = arr->numbers[i-1].squareVertices[1][0] + size        ;arr->numbers[i].squareVertices[2][1] = arr->numbers[i - 1].squareVertices[2][1];//(x3,y3)
        arr->numbers[i].squareVertices[3][0] = arr->numbers[i-1].squareVertices[1][0]               ;arr->numbers[i].squareVertices[3][1] = arr->numbers[i - 1].squareVertices[2][1];//(x4,y4)

    }  
    //Give coordinates for left and right arrays.
    if(arr->left!=NULL)
       giveCoordinates(arr->left,startX-(40*arr->length/2),startY-size-100);
    if (arr->right != NULL)
        giveCoordinates(arr->right, startX + (size*(arr->length/2))+ (40 * arr->length / 2), startY - size - 100);
}

//This method is used to construct a tree for the array used in merge sort;
void fillArrayTree(Array* arr) {
    if (!arr) //If array is empty, return.
        return;
    if (arr->length == 1){
        arr->isLeaf = true; //If any array has 1 number only, then it is a leaf.
        return;
    }
    Array* temp;

    //Left
    arr->left = new Array;
    temp = arr->left;
    *temp = Construct();
    temp->length = arr->length / 2;
    temp->values = new int[arr->left->length];
    for (int i = 0;i < arr->left->length;i++) {
        temp->values[i] = arr->values[i]; //Filling left array with the left half of the array;
    }

    //Right
    arr->right = new Array;
    temp = arr->right;
    *temp = Construct();
    temp->length = arr->length - (arr->length/2);
    temp->values = new int[arr->right->length];
    for (int i = 0;i < arr->right->length;i++) {
        temp->values[i] = arr->values[arr->left->length+i]; //Continue where left array left off by using its length;
    }

    fillArrayTree(arr->left);//Continue left tree.
    fillArrayTree(arr->right);//Continue right tree.
}

//This method is used to advance the visualization to the next step.
void nextStep() {

    //Check if the index of next array is less than the number of arrays stored.
    if (nextIndex <= startingArrayLength) {

        //If array is 1 number, it is sorted and there are no more steps.
        if (startingArray.length == 1)
            exit(0);

        //If array is 2 numbers, draw left and right nodes then go back to original.
        if (startingArray.length == 2&&nextIndex!=3)
        {   
            Current = nextIndex;
            nextIndex++;
            return;
        }
        if (isDone)
            exit(0);
        if (!isDone) {
                int temp; //This variable is used to traverse the tree backwards from the current array.
                isGoingBack = false;//By default, the tree is moving forwards, not backwards.

                //Checking of left and right of root are sorted.
                if (startingArray.left->isLeaf && startingArray.right->isLeaf){
                    startingArray.left->isSorted = true;
                    startingArray.right->isSorted = true;
                    startingArray.isLeaf = true;
                    startingArray.values = sortingArrays[sortingPointer++].values;
                    isDone = true;
                    isGoingBack = true;
                    Current = 0;
                    nextIndex--;
                }
            

                else if (startingArrays[Current]->isLeaf && !startingArrays[Current]->isSorted) {
                    temp = Current - 1;
                    //Check for the one behind current.
                    if (startingArrays[temp]->isLeaf && !startingArrays[temp]->isSorted) {
                         isGoingBack = true;
                        startingArrays[Current]->isSorted = true;
                        startingArrays[temp]->isSorted = true;
                        startingArrays[temp - 1]->isLeaf = true;
                        startingArrays[temp - 1]->values = sortingArrays[sortingPointer++].values;
                        Current = temp - 1;
                        nextIndex--;
                    }

                //Check if the current cell is different from next in line and if the one before the current is sorted.
                else if (Current != nextIndex-1 && startingArrays[temp]->isSorted) {

                    //Go through arrays to find the first unsorted leaf.
                    while (temp > 1) {
                        if (startingArrays[temp]->isLeaf && !startingArrays[temp]->isSorted) {
                            isGoingBack = true;
                            break;
                        }
                        temp--;
                    }
                    //If an unsorted leaf is found, go back to alter old arrays.
                    if (isGoingBack) {

                        //Current and previous unsorted leaves are marked as sorted.
                        startingArrays[Current]->isSorted = true;
                        startingArrays[temp--]->isSorted = true;

                        //Find the first array that is not a leaf.
                        while (startingArrays[temp]->isLeaf && !startingArrays[temp]->isSorted)
                            temp--;

                        //Mark the array to be changed as a leaf.
                        startingArrays[temp]->isLeaf = true;
                        //Give the leaf its sorted value.
                        startingArrays[temp]->values = sortingArrays[sortingPointer++].values;
                        //Current is the array that was changed and became a leaf.
                        Current = temp;
                        nextIndex--;
                    }
                }
            }
            //If not going back, the current array of interest is the next in line.
            if (!isGoingBack)
                Current = nextIndex;

            nextIndex++;
        }
    }
}
void mouse(int button, int state, int x, int y) {
    if ((button == GLUT_LEFT_BUTTON||button==GLUT_RIGHT_BUTTON) && state == GLUT_DOWN) {
        nextStep();
    }
    glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y) {

        if (key == 32)
        {
            nextStep();
        }

    glutPostRedisplay();
}

void setupArrays(Array *arr,Array *arrays[], int* pointer) {
    arrays[(*pointer)++] = arr;
    if (arr->left)
        setupArrays(arr->left,arrays,pointer);
    if (arr->right)
        setupArrays(arr->right,arrays,pointer);
}

int main(int argc, char** argv)
{  
    //Welcome to the Merge Sort Visualization project made by Hazem Ayman Ibrahim 202004410!
    //The program generates a random array with length n (ranging from 1 to 12) with random values and sorts them using merge sort.
    // To move onto the next step, press space or any mouse button.
    // When moving onto the next step when the sorting is done, the program closes.
    //If you want a constant length of the array, change (int)(rand() %12+1) to any constant value.
    

    //Setting up arrays.
    srand((unsigned int)time(NULL));
    int n = (int)rand()%12+3;float num = 0; //Range of n is from 3 to 12.
    size = 200* (10/double(n));
    if (size > 600)
        size = 600;
    else if (size < 200)
        size = 200;
    startingArray = Construct();
    sortedArray = Construct();
    startingArray.values = new int[n];
    sortedArray.values = new int[n];
    startingArray.length = n;
    sortedArray.length = n;

    for (int i = 0;i < startingArray.length;i++) {
        int sign = rand() % 2;
        switch (sign) {
        case 0: sign = 1;
            break;
        case 1: sign = -1;
            break;
        }
         num = rand();
        startingArray.values[i] = (int)num%10000*sign;
        sortedArray.values[i] = (int)num%10000*sign;
    }
    MergeSort(sortedArray); //Use sortedArray to sort to keep the startingArray unaffected when drawing.
    fillArrayTree(&startingArray); //Create a tree from of startingArray
    giveCoordinates(&startingArray,-(((double)n/2)*size), 900); //Give startingArray tree coordinates to draw them.
    sortingPointer = 0; //Reset Sorting arrays pointer for later use.
    setupArrays(&startingArray,startingArrays,&startingArrayLength);//Fill the startingArrays array of arrays to determine the order of drawing.

    //OpenGL Initialization.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1900,1000);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Merge Sort");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
}
