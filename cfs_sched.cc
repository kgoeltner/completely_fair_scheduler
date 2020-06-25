//
// Karl Goeltner
// 917006087
// ECS 36C - 05/22/2020
//
// cfs_sched.h - Implementation of the CFS Linux Kernel Scheduler.
// Receives a file containing a list of unordered task descriptions
// and reads in the tasks to run the CFS scheduler strategy until
// all tasks have reached completion.
//

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include "multimap.h"

// Task - class to represent a Task object
class Task {
 public:
    // Task() - Task Constructor for initialization
    explicit Task(char n, unsigned int ts, unsigned int d) :
    id(n), start_time(ts), duration(d) {}

    // ~Task() - Task Destructor
    ~Task(void) = default;

    // getID - return the task's id
    char getID(void) const {
      return id;
    }

    // getStartTime - return the task's start_time
    unsigned int getStartTime(void) const {
      return start_time;
    }

    // getvRuntime - return the task's vRuntime
    unsigned int getvRuntime(void) const {
      return vruntime;
    }

    // incRunTimes - increment runtime & vruntime of task
    void incRunTimes(void) {
      runtime++;
      vruntime++;
    }

    // setvRuntime - intialize virtual runtime to current global min_vruntime
    void setvRuntime(unsigned int global_time) {
      // Inherit same priority as the next schedulable task
      vruntime = global_time;
    }

    // isComplete - check if vruntime is equal to duration for completion
    bool isComplete(void) {
      if (runtime == duration)
        return true;
      return false;
    }

    // operator<< - overload the operator<< to print out Task values
    friend std::ostream& operator<<(std::ostream& os, const Task& t) {
      os << t.id << " " << t.start_time << " " << t.duration <<
        " vruntime:" << t.vruntime << " runtime:" << t.runtime << std::endl;
      return os;
    }

 private:
    // Variables to id, tick starting point, duration
    char id;
    unsigned int start_time;
    unsigned int duration;

    // Variables to track running time and vrunning time
    unsigned int runtime = 0;
    unsigned int vruntime = 0;
};

// Scheduler - class to represent a CFL scheduler object
class Scheduler {
 public:
    // Scheduler() - Scheduler Constructor for initialization
    explicit Scheduler(std::vector<Task*>& tasks) :
        min_vruntime(0), tick_counter(0), completed(0), task_list(tasks) {}

    // ~Scheduler() - Scheduler Destructor
    ~Scheduler(void) = default;

    // appendTimeline - if tasks to be launched at tick value, add to timeline
    void appendTimeline(void) {
      // Go through all tasks and append to timeline if start time matches
      for (auto task : task_list) {
        if (task->getStartTime() == tick_counter) {
          task->setvRuntime(min_vruntime);
          timeline.Insert(task->getvRuntime(), task);
        }
      }
    }

    // moveNextTask - check if currently running task should transfer to next
    void moveNextTask(void) {
      // As long as timeline not empty & current task running,
      // check if timeline -> to next task
      if (!empty() && current_task && current_task->getvRuntime()
        > min_vruntime) {
        timeline.Insert(current_task->getvRuntime(), current_task);
        current_task = nullptr;
      }
    }

    // getNextTask - if current task stopped, get next schedulable task
    void getNextTask(void) {
      // If timeline isn't empty, get next task
      if (current_task == nullptr && !empty()) {
        // Obtain min vruntime task
        current_task = timeline.Get(timeline.Min());
        // Remove current task from timeline
        timeline.Remove(current_task->getvRuntime());
        // If not empty, set global min_vruntime to next task's vruntime
        if (!empty())
          min_vruntime = timeline.Min();
      }
    }

    // incremenTask - current task runs for one tick
    void incrementTask(void) {
      // As long as current task is running, ++task's runtime & vruntime
      if (current_task)
        current_task->incRunTimes();
    }

    // printStatus - print current scheduling status on screen
    void printStatus(void) {
      // <tick> [<#tasks>]: <ID of running task>
      std::cout << tick_counter << " [" << runningTasks()
        << "]: ";

      // As long as current task is running, print out task id
      if (current_task) {
        std::cout << current_task->getID();
        // Print the * if the task has reached completion
        if (current_task->isComplete())
          std::cout << "*";
      // Else print out _ for no task
      } else {
        std::cout << "_";
      }
      // Print end of line
      std::cout << std::endl;
    }

    // purgeCompletion - if current task has completed, purge from system
    void purgeCompletion(void) {
      // As long as current task is running & is complete -> remove
      if (current_task && current_task->isComplete()) {
        // Increment compeleted tasks counter
        completed++;
        // Delete object entirely & set current task null
        delete current_task;
        current_task = nullptr;
      }
    }

    // incrementTick - increment tick value by one so loop can restart
    void incrementTick(void) {
      tick_counter++;
    }

    // done - return true if all tasks are completed
    bool done(void) {
      return completed == task_list.size();
    }

 private:
    // Global min_vruntime
    unsigned int min_vruntime;
    // Tick counter
    unsigned int tick_counter;
    // Completed tasks counter
    unsigned int completed;
    // Vector to hold all read-in file tasks
    std::vector<Task*> task_list;
    // RB-tree multimap to hold timeline of tasks
    Multimap<int, Task*> timeline;
    // Currently running task
    Task* current_task = nullptr;

    // empty - return true if multimap is empty
    bool empty(void) {
      return timeline.Size() == 0;
    }

    // runningTasks - return total # of running tasks
    unsigned int runningTasks(void) {
      unsigned int running_count = timeline.Size();
      // If a task is currently running, increment
      if (current_task != nullptr)
        running_count++;
      return running_count;
    }
};

// checkFileStream - perform error-checking on a generic file-stream
template<typename T>
void checkFileStream(const T& file, char* file_name) {
  // Check if able to open file
  if (!file.is_open()) {
    std::cerr << "Error: cannot open file " << file_name << std::endl;
    exit(1);
  }
}

// storeData - store Data objects into vector
void storeData(std::vector<Task*>& task_list, std::ifstream& in_file) {
  // Variables to temporarily store a Task id, start time, duration
  char id;
  unsigned int start_time;
  unsigned int duration;

  // Store each line's id, start time, duration into Task object
  while (in_file >> id >> start_time >> duration)
      task_list.push_back(new Task(id, start_time, duration));

  in_file.close();
}

// alphaOrder - if tasks have equal start_time, order by id character
bool alphaOrder(const Task* t1, const Task* t2) {
  if (t1->getStartTime() == t2->getStartTime())
    return t1->getID() < t2->getID();
  // Otherwise, order by start time
  return t1->getStartTime() < t2->getStartTime();
}

// organizeTasks - rearrange tasks so tasks with the same
//                 start time run in alphabetical order ID
void organizeTasks(std::vector<Task*>& task_list) {
  std::sort(task_list.begin(), task_list.end(), alphaOrder);
}

// runCFS - run the CFS algorithm using a RB-Tree multimap
void runCFS(std::vector<Task*>& task_list) {
  // Scheduler object to handle timeline of tasks
  Scheduler cfs(task_list);

  // CFS Algorithm
  do {
    // 1) If tasks to be launched at tick value, add to timeline
    cfs.appendTimeline();
    // 2) Check if currently running task should transfer to next task
    cfs.moveNextTask();
    // 3) If current task stopped running, get next task
    cfs.getNextTask();
    // 4) Current task runs for one tick
    cfs.incrementTask();
    // 5) Report scheduling status
    cfs.printStatus();
    // 6) If current task has completed, purge from system
    cfs.purgeCompletion();
    // 7) Increment tick value by one, loop restarts
    cfs.incrementTick();
  // Keep running until all tasks are completed
  } while (!cfs.done());
}

// Main method
int main(int argc, char *argv[]) {
  std::vector<Task*> task_list;

  // Make sure correct number of command-line arguments are present
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <task_file.dat>" << std::endl;
    exit(1);
  }

  // Open data file
  std::ifstream data_file(argv[1]);

  // Check that data file opens properly
  checkFileStream(data_file, argv[1]);

  // Store data tasks into task_list
  storeData(task_list, data_file);

  // Organize data tasks with equal start_time in alphabetical order
  organizeTasks(task_list);

  // Run CFS scheduler strategy until completion
  runCFS(task_list);

  return 0;
}
