#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 10
// Define the Channel enum
enum Channel { SPORTS, ENTERTAINMENT, ECONOMICS, POLITICS, UNSORTED };

// Define the MicroMsg structure
struct MicroMsg {
  int PostID;
  int PostDate; // Represented as an integer (UNIX timestamp)
  char
      PostBody[141]; // 140 characters for the post body + 1 for null terminator
  char UserName[50]; // You can adjust the size as per your requirements
  enum Channel Channel;
  struct MicroMsg *next;
};

// Function prototypes
struct MicroMsg *createMicroMsg(int postDate, const char *postBody,
                                const char *userName, enum Channel channel,
                                int POSTID) {

  struct MicroMsg *message = (struct MicroMsg *)malloc(sizeof(struct MicroMsg));

  message->PostID = POSTID;
  (*message).PostDate = postDate;
  strcpy((*message).PostBody, postBody);
  strcpy((*message).UserName, userName);
  (*message).Channel = channel;
  (*message).next = NULL;
  return message;
}
void insertMicroMsgAtBeginning(struct MicroMsg **head,
                               struct MicroMsg *newMicroMsg) {

  newMicroMsg->next = *head;
  *head = newMicroMsg;
}

void deleteMicroMsg(struct MicroMsg **head, int postIDToDelete) {
  if (*head == NULL) {
    return;
  }
  if ((*head)->PostID == postIDToDelete) {
    struct MicroMsg *temp = *head;
    *head = (*head)->next;
    free(temp);
    return;
  }
  struct MicroMsg *current = *head;
  while (current != NULL) {
    if ((*((*current).next)).PostID == postIDToDelete) {
      struct MicroMsg *temp = current->next;
      current->next = current->next->next;
      free(temp);
      break;
    }
    current = current->next;
  }
}
void displayMicroMsgs(struct MicroMsg *head) {
  while (head != NULL) {
    printf("POST ID: %d\nPOST DATE: %d\nPOST BODY: %s\nUSERNAME: %s\nCHANNEL: "
           "%d\n\n",
           head->PostID, head->PostDate, head->PostBody, head->UserName,
           head->Channel);
    head = head->next;
  }
  printf("--------------\n\n");
}
void freeMicroMsgs(struct MicroMsg *head) { // Function to free the memory
                                            // allocated for the linked list
  struct MicroMsg *temp = head;
  while (head != NULL) {
    temp = head;
    head = head->next;
    free(temp);
  }
}

// Structure to store channel statistics
struct ChannelStats {
  int numPosts;
  int sumCharacters;
};

// Structure to store user name and post count
struct UserNameCount {
  char UserName[50];
  int count;
};

struct ChannelStats *getChannelStats(struct MicroMsg *head) {
  struct ChannelStats *stats =
      (struct ChannelStats *)malloc(5 * sizeof(struct ChannelStats));
  int spor = 0;
  int ent = 0;
  int econ = 0;
  int pol = 0;
  int uns = 0;

  int charspor = 0;
  int charent = 0;
  int charecon = 0;
  int charpol = 0;
  int charuns = 0;

  while (head != NULL) {

    switch (head->Channel) {

    case 0:
      spor++;
      charspor = charspor + strlen(head->PostBody);
      break;

    case 1:
      ent++;
      charent = charent + strlen(head->PostBody);
      break;

    case 2:
      econ++;
      charecon = charecon + strlen(head->PostBody);
      break;

    case 3:
      pol++;
      charpol = charpol + strlen(head->PostBody);
      break;

    case 4:
      uns++;
      charuns = charuns + strlen(head->PostBody);
      break;
    }

    head = head->next;
  }
  stats[0].numPosts = spor;
  stats[0].sumCharacters = charspor;
  stats[1].numPosts = ent;
  stats[1].sumCharacters = charent;
  stats[2].numPosts = econ;
  stats[2].sumCharacters = charecon;
  stats[3].numPosts = pol;
  stats[3].sumCharacters = charpol;
  stats[4].numPosts = uns;
  stats[4].sumCharacters = charuns;

  return stats;
}

int sizeoflinkedlist(struct MicroMsg *head) {
  int count = 0;
  while (head != NULL) {
    head = head->next;
    count++;
  }
  return count;
}

struct UserNameCount* CheckNumberofUsers(struct MicroMsg *head, enum Channel ID,
                                         int sizeoflist) {

  struct UserNameCount *ChannelArr = (struct UserNameCount *)calloc(
      sizeoflist, sizeof(struct UserNameCount)); // allocate memory for each array and set
                                                 // each element to 0

  struct MicroMsg *current = head;
  while (current != NULL) { // goes through each Node
    int check = 0;
    if (current->Channel == ID) { // checks if the nodes channel is equal to the
                                  // channel that is being looked for
      for (int i = 0; i < sizeoflist; i++) { // checks if username already in the array
        if (strcmp(ChannelArr[i].UserName, current->UserName) == 0) {
          ChannelArr[i].count++; // if it is increment
          check++; // indicates that username is already in the array
        }
      }
      if (check != 0) { // checks to see if username needs to be added to array
        check = 0;      // reset the check variable to 0
      } else {
        for (int j = 0; j < sizeoflist; j++) { // check for vacant spot in new array
          if (ChannelArr[j].count == 0) { // put this username into the new
                                          // array
            strcpy(ChannelArr[j].UserName, current->UserName);
            ChannelArr[j].count++; // puts number of posts from this user to 1
            break;
          }
        }
      }
    }

    current = current->next; // move to the next node
  }
  return ChannelArr;
}

struct UserNameCount *StoreTopContributors(struct UserNameCount *channel, int k,
                                           int sizeoflist) {

  for (int j = 0; j < k;
       j++) { // for the number less then the number of top contributors
    struct UserNameCount max = channel[j]; // assume first element is the max
    for (int i = j; i < sizeoflist;
         i++) { // goes through each element starting from k
      if (max.count < channel[i].count) { // change max to the highest element
        max.count = channel[i].count;
        strcpy(max.UserName, channel[i].UserName);
      }
    }
    channel[j].count =
        max.count; // assorts array from highest number to lowest number
    strcpy(channel[j].UserName, max.UserName);
  }
  return channel;
}

struct UserNameCount *getTopContributorsForChannel(struct MicroMsg *head,
                                                   int k) {

  struct UserNameCount *TopCont =
      (struct UserNameCount *)malloc(k * 5 * sizeof(struct UserNameCount));
  if (head == NULL) {
    return TopCont;
  }
  // function to check the number of users in each channel and stores them in
  // array
  struct UserNameCount *econ =
      CheckNumberofUsers(head, 0, sizeoflinkedlist(head));
  struct UserNameCount *ent =
      CheckNumberofUsers(head, 1, sizeoflinkedlist(head));
  struct UserNameCount *spor =
      CheckNumberofUsers(head, 2, sizeoflinkedlist(head));
  struct UserNameCount *pol =
      CheckNumberofUsers(head, 3, sizeoflinkedlist(head));
  struct UserNameCount *uns =
      CheckNumberofUsers(head, 4, sizeoflinkedlist(head));

  // function to store top contributors for each channel and sorts them
  econ = StoreTopContributors(econ, k, sizeoflinkedlist(head));
  ent = StoreTopContributors(ent, k, sizeoflinkedlist(head));
  spor = StoreTopContributors(spor, k, sizeoflinkedlist(head));
  pol = StoreTopContributors(pol, k, sizeoflinkedlist(head));
  uns = StoreTopContributors(uns, k, sizeoflinkedlist(head));

  // insert each element of TopCont to corresponding element
  int count = 0;
  for (int i = 0; i < k; i++) {
    TopCont[count] = econ[i];
    count++;
  }
  for (int i = 0; i < k; i++) {
    TopCont[count] = ent[i];
    count++;
  }
  for (int i = 0; i < k; i++) {
    TopCont[count] = spor[i];
    count++;
  }
  for (int i = 0; i < k; i++) {
    TopCont[count] = pol[i];
    count++;
  }
  for (int i = 0; i < k; i++) {
    TopCont[count] = uns[i];
    count++;
  }

  return TopCont;
}

int main() {
  // Test your linked list operations here
  struct MicroMsg *head = NULL;

  // Create and insert MicroMsg posts
  struct MicroMsg *msg1 = createMicroMsg(
      1629600000, "Hello, this is my first post!", "user1", SPORTS, 2);
  insertMicroMsgAtBeginning(&head, msg1);

  struct MicroMsg *msg2 = createMicroMsg(
      1629686400, "Just chilling with friends.", "user2", ENTERTAINMENT, 3);
  insertMicroMsgAtBeginning(&head, msg2);

  struct MicroMsg *msg3 = createMicroMsg(1629772800, "Economy is doing well.",
                                         "user3", ECONOMICS, 1);
  insertMicroMsgAtBeginning(&head, msg3);

  // Display and test other functions
  displayMicroMsgs(head);

  // Delete a post with PostID 2
  deleteMicroMsg(&head, 2);
  displayMicroMsgs(head);

  // Get channel statistics
  struct ChannelStats *stats = getChannelStats(head);
  printf("Channel Statistics:\n");
  printf("Sports: Number of Posts: %d, Sum of Characters: %d\n",
         stats[SPORTS].numPosts, stats[SPORTS].sumCharacters);
  printf("Entertainment: Number of Posts: %d, Sum of Characters: %d\n",
         stats[ENTERTAINMENT].numPosts, stats[ENTERTAINMENT].sumCharacters);
  printf("Economics: Number of Posts: %d, Sum of Characters: %d\n",
         stats[ECONOMICS].numPosts, stats[ECONOMICS].sumCharacters);
  printf("Politics: Number of Posts: %d, Sum of Characters: %d\n",
         stats[POLITICS].numPosts, stats[POLITICS].sumCharacters);
  printf("Unsorted: Number of Posts: %d, Sum of Characters: %d\n\n\n",
         stats[UNSORTED].numPosts, stats[UNSORTED].sumCharacters);

  // Find top contributors for each channel (let's say top 2 contributors)
  int k = 2;
  struct UserNameCount *topContributors = getTopContributorsForChannel(head, k);
  for (int i = 0; i < 5 * k; i++) {
    printf("Channel %d, Top Contributor %d: UserName: %s, Count: %d\n", i / k,
           i % k + 1, topContributors[i].UserName, topContributors[i].count);
  }

  // Free memory when done
  freeMicroMsgs(head);
  head = NULL;

  return 0;
}
