#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define CANDIDATE_COUNT 4

#define CANDIDATE1 "Kewal Dahal"
#define CANDIDATE2 "Bipin Giri"
#define CANDIDATE3 "Prakhar Katwal"
#define CANDIDATE4 "Sandesh Bharti"

struct Candidate {
  char name[50];
  char party[50];
};

typedef struct Candidate Candidate;

Candidate candidates[CANDIDATE_COUNT];
int votesCount[CANDIDATE_COUNT] = {0};
int spoiledVotes = 0;

// Function to load candidate names and parties into the candidates array
void loadCandidates() {
  strcpy(candidates[0].name, CANDIDATE1);
  strcpy(candidates[1].name, CANDIDATE2);
  strcpy(candidates[2].name, CANDIDATE3);
  strcpy(candidates[3].name, CANDIDATE4);

  strcpy(candidates[0].party, "Party A");
  strcpy(candidates[1].party, "Party B");
  strcpy(candidates[2].party, "Party C");
  strcpy(candidates[3].party, "Party D");
}

// Function to clear the console screen (system-dependent)
void clearScreen() {
  system("cls"); // For Windows. Use "clear" for Linux/macOS.
}

// Function to display the list of candidates
void displayCandidates() {
  printf("\n\n ### List of Candidates ###\n");
  for (int i = 0; i < CANDIDATE_COUNT; i++) {
    printf(" %d. %s (%s)\n", i + 1, candidates[i].name, candidates[i].party);
  }
  printf(" %d. None of These\n", CANDIDATE_COUNT + 1);
}

// Function to get the current time as a string
char *getCurrentTime() {
  time_t now;
  time(&now);
  return ctime(&now);
}

// Function to check if a voter has already voted
int isVoterAlreadyVoted(const char *voterID) {
  FILE *file = fopen("voted.txt", "r");
  if (file == NULL) {
    printf("Error opening voted ID file.\n");
    return 0;
  }

  char line[100];
  while (fgets(line, sizeof(line), file)) {
    if (line[strlen(line) - 1] == '\n') {
      line[strlen(line) - 1] = '\0';
    }

    if (strstr(line, voterID) != NULL) {
      fclose(file);
      return 1;
    }
  }

  fclose(file);
  return 0;
}

// Function to check if a voter is valid
int isValidVoter(const char *voterID) {
  FILE *file = fopen("votingid.txt", "r");
  if (file == NULL) {
    printf("Error opening voter ID file.\n");
    return 0;
  }

  char line[100];
  while (fgets(line, sizeof(line), file)) {
    if (line[strlen(line) - 1] == '\n') {
      line[strlen(line) - 1] = '\0';
    }

    if (strcmp(line, voterID) == 0) {
      fclose(file);
      return 1;
    }
  }

  fclose(file);
  return 0;
}

// Function to save vote details to a file
void saveVoteDetailsToFile(const char *voterID, int candidateIndex) {
  FILE *file = fopen("voting_details.txt", "a");
  if (file == NULL) {
    printf("Error opening file for writing.\n");
    return;
  }

  fprintf(file, "%s - %s\n", candidates[candidateIndex].name, getCurrentTime());

  fclose(file);
}

// Function to update vote count in the voting_details.txt file
void updateVoteCountToFile() {
  FILE *file = fopen("voting_details.txt", "w"); // Open in write mode to clear the file
  if (file == NULL) {
    printf("Error opening file for writing.\n");
    return;
  }

  fprintf(file, "\t\t\t||||********************************************************||||\t\t\t\n");

  for (int i = 0; i < CANDIDATE_COUNT; i++) {
    fprintf(file, "%s - %d votes\n", candidates[i].name, votesCount[i]);
  }

  fprintf(file, " Spoiled Votes - %d\n", spoiledVotes);

  fclose(file);
}

// Function for casting a vote
void castVote() {
  clearScreen();
  char voterID[20];
  printf("\n\n ### Please enter your Voter ID to cast your vote: ");
  scanf("%s", voterID);

  if (isVoterAlreadyVoted(voterID)) {
    printf("\n You have already cast your vote!\n");
    sleep(2);
    return;
  }

  if (!isValidVoter(voterID)) {
    printf("\n Invalid Voter ID! Vote casting failed.\n");
    sleep(2);
    return;
  }

  displayCandidates();

  int choice;
  printf("\n Input your choice (1 - %d): ", CANDIDATE_COUNT + 1);
  scanf("%d", &choice);

  if (choice >= 1 && choice <= CANDIDATE_COUNT) {
    votesCount[choice - 1]++;
    printf("\n Thanks for voting!\n");

    FILE *votedFile = fopen("voted.txt", "a");
    if (votedFile == NULL) {
      printf("Error opening voted ID file for writing.\n");
      sleep(2);
      return;
    }
    fprintf(votedFile, "%s - Voted for %s (%s)\n", voterID,
            candidates[choice - 1].name, getCurrentTime());
    fclose(votedFile);

    saveVoteDetailsToFile(voterID, choice - 1);
    updateVoteCountToFile(candidates[choice - 1].name);
    sleep(2);
  } else if (choice == CANDIDATE_COUNT + 1) {
    spoiledVotes++;
    printf("\n Your vote has been marked as spoiled.\n");
    sleep(2);
  } else {
    printf("\n Error: Wrong Choice! Please retry.\n");
    sleep(2);
  }
}

// Function to pause the screen until Enter key is pressed
void waitForEnter() {
  printf("\nPress Enter to continue...");
  while (getchar() != '\n');
  getchar();  // Consume the Enter key
}

// Function to print voting statistics
void printVotesCount() {
  clearScreen();
  printf("\n\n ##### Voting Statistics  ####\n");

  // Open the "voting_details.txt" file in read mode
  FILE *votingDetailsFile = fopen("voting_details.txt", "r");
  if (votingDetailsFile == NULL) {
    printf("Error opening voting_details.txt file.\n");
  } else {
    char line[100];
    while (fgets(line, sizeof(line), votingDetailsFile)) {
      printf("%s", line);
    }
    fclose(votingDetailsFile);
  }

  waitForEnter();  // Wait for Enter key
}


// Function to find the leading candidate
void getLeadingCandidate() {
  clearScreen();
  printf("\n\n  #### Leading Candidate ####\n\n");
  int maxVotes = votesCount[0];
  int leadingCandidateIndex = 0;

  for (int i = 1; i < CANDIDATE_COUNT; i++) {
    if (votesCount[i] > maxVotes) {
      maxVotes = votesCount[i];
      leadingCandidateIndex = i;
    }
  }

  if (maxVotes > 0) {
    printf("[%s]\n", candidates[leadingCandidateIndex].name);
  } else {
    printf("----- Warning !!! No-win situation ----\n");
  }
  waitForEnter();
}

// Function to display a loading animation
void displayLoading(int delay) {
  printf("Loading");
  fflush(stdout);
  for (int i = 0; i < 3; i++) {
    usleep(delay);
    printf(".");
    fflush(stdout);
  }
  usleep(delay);
  printf("\n");
}

int main() {
  int choice;

  loadCandidates();

  do {
    clearScreen();
    printf("                                    ***************************************\n");
    printf("                                    *                                     *\n");
    printf("                                    *  Welcome to Election/Voting 2023    *\n");
    printf("                                    *                                     *\n");
    printf("                                    ***************************************\n");

    printf("\n\n 1. Cast your Vote");
    printf("\n 2. Find Vote Count");
    printf("\n 3. Find Leading Candidate");
    printf("\n 0. Exit");

    printf("\n\n Please enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
      displayLoading(300000);
      castVote();
      break;
    case 2:
      displayLoading(200000);
      printVotesCount();
      break;
    case 3:
      displayLoading(200000);
      getLeadingCandidate();
      break;
    case 0:
      printf("\n Exiting the program. Goodbye!\n");
      break;
    default:
      printf("\n Error: Invalid Choice\n");
    }

  } while (choice != 0);

  return 0;
}