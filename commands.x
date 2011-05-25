enum Day (SUN, MON, TUE, WED, THU, FRI, SAT)

typedef {uint16 year, uint8 month, uint8 day, uint8 hour, uint8 minute, uint8 second} Date
typedef {bool passed, string errorMessage} Result

section "Common commands"

"Whatever you do DONT LOG IN!!!"
Result login(string userName, string password)

"Whatever you do DONT LOG OUT!!!"
logout()

"Now, why would you do that?"
quit()

"Display help on a command.  If command is null, just list commands."
string helpCommand(string commandName)

"Just says wether or not you are logged in."
bool loggedIn()

"Returns the amount of money in your account, in cents"
int32 balance()

"Send money to another account."
Result send(string userName, int32 amount)

"List transactions"
[{Date date, string userName, int32 amount}] transactions(Date fromDate = {0,0,0,0,0,0},
    Date toDate = {0,0,0,0,0,0})

section "For manipulating accounts"

"Create a new user"
Result newUser(string userName, string shownName, string email, string password)

"Update account information."
Result updateAccount(string userName, string shownName, string email, string oldPassword,
    string password = null)

"Get account settings"
{string userName, string shownName, string email} accountSettings()

"Validate a new user"
Result validateUser(string userName, string key)

"Delete a user account"
Result deleteMyAccount()

"Reset a user's password"
Result resetPassword(string userName)

section "Forums"

"Create a new thread"
Result newPost(string subject, string message)

"List threads in the forum"
[{int32 threadID, string subject, int32 numPosts}] listThreads()

"Show posts in a thread"
[{string shownName, string post}] showPosts(int32 threadID, int32 startPost,
int32 endPost)

"Reply to a post"
Result reply(int32 threadID, int32 postNumber, string message)

"Start following a thread"
Result followThread(int32 threadID)

"Stop following a thread"
Result leaveThread(int32 threadID)

section "Administrator only commands"

"Write out the entire database to an ASCII file"
Result saveDatabase()
