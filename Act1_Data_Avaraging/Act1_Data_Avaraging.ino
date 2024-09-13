#define Sound_sensor A0

// Global Variables
int raw_signal = 0, new_signal = 0, Min = 0, Max = 0,Mode = 0; //data Receivers
int running_ave[10]; // Array to store the average values
int raw_arr[10]; // Array to store normal values 
int moving_data;     // To store the result of the moving average function
int Mode_data; 
int i = 0, x = 0, index = 0;    // Counters
unsigned long Timer;// timer 
const unsigned long timeout = 6000; // timeout duration

void setup() {
  Serial.begin(9600);
  Timer = millis();
}

void loop() {
  // interupt the loop after 6 seconds 
  if(millis() - Timer > timeout){
    delay(100);
    Serial.println("End of receiving data");
    while(true){
    }
  }

  raw_signal = analogRead(Sound_sensor);
   // Store raw raw_signal in array
  raw_arr[index] = raw_signal;
  index++; // Move to the next position in the array
  // If index is maxed reset to 0
  if (index == 10) {
    index = 0; 
  }

  // Call Moving average function
  moving_data = Moving_Ave(raw_signal);
  // Call GetMinMax
  GetMinMax(raw_arr, 10, Min, Max); 
  //call GetMode
  Mode_data = GetMode(raw_arr, 10);;

  // Print both raw signal and running average to the Serial Plotter
  Serial.print(raw_signal); //raw data
  Serial.print("\t"); 
  Serial.print(moving_data); //ave
  Serial.print("\t");
  Serial.print(Min); //minima
  Serial.print("\t");
  Serial.print(Max); //maxima 
  Serial.print("\t");
  Serial.println(Mode_data); // mode 

  delay(100); 
}

// Moving Average method 
int Moving_Ave(int raw_signal) {
  new_signal += raw_signal;
  // Increment i every time a new raw signal is added
  i++;
  // If i reaches 10, calculate the moving average
  if (i == 10) {
    running_ave[x] = new_signal / 10; // Store the average in the array
    x++; // Move to the next position in the array
    // Reset i and new_signal 
    new_signal = 0;
    i = 0;
    // If x is 10 reset to 0
    if (x == 10) {
      x = 0;
    }
  }
  // Return the most recent average value
  return running_ave[(x == 0) ? 9 : x - 1]; 
}

void GetMinMax(int raw_arr[], int size, int &Min, int &Max) {
  Min = raw_arr[0];
  Max = raw_arr[0];
  for (int i = 1; i < size; i++) {
    if (raw_arr[i] < Min) {
      Min = raw_arr[i];
    }
    if (raw_arr[i] > Max) {
      Max = raw_arr[i];
    }
  }
}

int GetMode(int raw_arr[], int size) {
  int mode = raw_arr[0];
  int max_count = 1;

  for (int i = 0; i < size; i++) {
    int count = 0;

    // Count occurrences of raw_arr[i]
    for (int j = 0; j < size; j++) {
      if (raw_arr[j] == raw_arr[i]) {
        count++;
      }
    }

    // Update mode if a higher count is found
    if (count > max_count) {
      max_count = count;
      mode = raw_arr[i];
    }
  }
  return(mode);  
}

