#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27,  16, 2);


#define MAX_NUMBERS 10  // Define the maximum number of elements you want to sort
int myArray[MAX_NUMBERS];


volatile int order=0;
volatile char incomingChar;

// Measure time elapse
unsigned long startTime = 0;
unsigned long endTime = 0;
unsigned long elapsedTime = 0;
volatile int sizeOfArray;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  randomSeed(analogRead(0)); // Seed the random number generator
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Rand(R) | Input?");
  Serial.println("Write command");

  while (!Serial.available()) {
    // Wait for user input
  }
  incomingChar = Serial.read();

  if (incomingChar == 'R' || incomingChar == 'r') { // Si se van a generar numeros aleatorios
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Size of array:");
    Serial.println("Type size of random numbers");
    while (!Serial.available()) {
    // Wait for user input
    }
    int n = Serial.parseInt(); // Read the user input as an integer
    int* numbers = generateUniqueRandomNumbers(n); //Se genera al puntero de numeros random generados
    sort(numbers,n);
    delete[] numbers; // Free the dynamically allocated memory
  }
  else{ //Insertar los valores 1x1
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Enter numbers");
    lcd.setCursor(0, 1);
    for (int i = 0; i<10; i++) {
      Serial.print("Insert number: "); Serial.println(i+1);
      while (!Serial.available()) {
    // Wait for user input
      }
      incomingChar = Serial.read();
      if(incomingChar == 'E' || incomingChar == 'e'){
        break;
      }
      myArray[i] = incomingChar - '0';
      lcd.print(myArray[i]);
      order = i+1;
      
    }
    delay(300);
    sort(myArray,order);
  }
}


int* generateUniqueRandomNumbers(int n) {
  int* numbers = new int[n];
  for (int i = 0; i < n; ++i) {
    bool unique;
    do {
      unique = true;
      numbers[i] = random(-500, 501);
      // Check if the generated number is already in the array
      for (int j = 0; j < i; ++j) {
        if (numbers[j] == numbers[i]) {
          unique = false;
          break;
        }
      }
    } while (!unique);
  }
  return numbers;
}

void sort(int num[], int size){
  
  if (size == 0) {
  sizeOfArray = findSizeOfArray(sizeof(num), num);
  }
  else{
  sizeOfArray = size;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("A,B,C or #");
  while (1)
  {
  while (!Serial.available()) {
    // Wait for user input
  }
  incomingChar = Serial.read();
  lcd.clear();
  lcd.setCursor(0, 0);

  if (incomingChar == 'A' || incomingChar == 'a') { // Sort function
    lcd.print("Bubble Sort:");
    delay(300);
    elapsedTime = 0;
    myBubbleSort(num, sizeOfArray);
    elapsedTime = endTime - startTime;
    displayResults(num, sizeOfArray, elapsedTime);
  }
  else if (incomingChar == 'B' || incomingChar == 'b') { // Insertion
    lcd.print("Insertion Sort:");
    delay(300);
    elapsedTime = 0;
    myInsertionSort(num, sizeOfArray);
    elapsedTime = endTime - startTime;
    displayResults(num, sizeOfArray, elapsedTime);
  }
  else if (incomingChar == 'C' || incomingChar == 'c') { //Quick sort
    lcd.print("Quick Sort:");
    delay(300);
    elapsedTime = 0;
    quickSort(num, 0, sizeOfArray -1);
    elapsedTime = endTime - startTime;
    displayResults(num, sizeOfArray, elapsedTime);
  }
  else if (incomingChar == '#') {
  break;
  }
  }
}

void myBubbleSort(int bubbleArray[], int arraySize)
{
  startTime = micros();
  for (int index = 0; index < arraySize - 1; ++index) {
    for (int i = 0; i < arraySize - index - 1; ++i) {
      if (bubbleArray[i] > bubbleArray[i + 1]) {
        int temp = bubbleArray[i];
        bubbleArray[i] = bubbleArray[i + 1];
        bubbleArray[i + 1] = temp;
      }
    }
  }
  endTime = micros();
}

void myInsertionSort(int insertionArray[], int arraySize) {
  startTime = micros();
  for (int i = 1; i < arraySize; i++) {
    int key = insertionArray[i];
    int j = i - 1;
	// Move elements of arr[0..i-1] that are greater than key
    // to one position ahead of their current position
    while (j >= 0 && insertionArray[j] > key) {
      insertionArray[j + 1] = insertionArray[j];
      j = j - 1;
    }
    insertionArray[j + 1] = key;
  }
  endTime = micros();
}

void quickSort(int array[], int low, int high) {
  startTime = micros();
  if (low < high) {
    
    // find the pivot element such that
    // elements smaller than pivot are on left of pivot
    // elements greater than pivot are on right of pivot
    int pi = partition(array, low, high);
    
    // recursive call on the left of pivot
    quickSort(array, low, pi - 1);
    
    // recursive call on the right of pivot
    quickSort(array, pi + 1, high);
  }
  endTime = micros();
}

int partition(int array[], int low, int high) {
  
  // select the rightmost element as pivot
  int pivot = array[high];
  
  // pointer for greater element
  int i = (low - 1);

  // traverse each element of the array
  // compare them with the pivot
  for (int j = low; j < high; j++) {
    if (array[j] <= pivot) {
        
      // if element smaller than pivot is found
      // swap it with the greater element pointed by i
      i++;
      
      // swap element at i with element at j
      swap(&array[i], &array[j]);
    }
  }

  // swap the pivot element with the greater element at i
  swap(&array[i + 1], &array[high]);
  
  // return the partition point
  return (i + 1);
}

void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

void displayResults(int arrayData[], int sizeOfData, unsigned long timeData) 
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" R E S U L T S ");
  delay(300);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(timeData);
  lcd.print(" us");
  lcd.setCursor(0, 1);
  if (sizeOfData <= 10){
  for (int i = 0; i < sizeOfData; i++) {
    lcd.print(arrayData[i]);
    //if (i < sizeOfData - 1) {
    //  lcd.print(", ");
    //}
  }
  }
}

int findSizeOfArray(int numOfBytes, int myVector[])
{
  int sizeOfVector = numOfBytes/sizeof(myVector[0]);
  return sizeOfVector;
}