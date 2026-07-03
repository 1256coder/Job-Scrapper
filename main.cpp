#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

/*
==================================================
COLOR SCHEME
==================================================
*/

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

/*
==================================================
JOB STRUCTURE
==================================================
*/

struct Job
{
    string title;
    string company;
    string category;
    string link;
};

vector<Job> jobs;

/*
==================================================
LOWERCASE FUNCTION
==================================================
*/

string toLower(string text)
{
    transform(text.begin(),
              text.end(),
              text.begin(),
              ::tolower);

    return text;
}

/*
==================================================
CURL CALLBACK
==================================================
*/

size_t WriteCallback(void* contents,
                     size_t size,
                     size_t nmemb,
                     string* output)
{
    size_t totalSize = size * nmemb;

    output->append((char*)contents,
                   totalSize);

    return totalSize;
}

/*
==================================================
FETCH JOB DATA
==================================================
*/

string fetchData()
{
    CURL* curl;

    string readBuffer;

    curl = curl_easy_init();

    if(curl)
    {
        curl_easy_setopt(curl,
                         CURLOPT_URL,
                         "https://remoteok.com/api");

        curl_easy_setopt(curl,
                         CURLOPT_WRITEFUNCTION,
                         WriteCallback);

        curl_easy_setopt(curl,
                         CURLOPT_WRITEDATA,
                         &readBuffer);

        curl_easy_setopt(curl,
                         CURLOPT_USERAGENT,
                         "Mozilla/5.0");

        curl_easy_perform(curl);

        curl_easy_cleanup(curl);
    }

    return readBuffer;
}

/*
==================================================
LOAD JOBS
==================================================
*/

void loadJobs()
{
    jobs.clear();

    string data = fetchData();

    auto jsonData = json::parse(data);

    /*
    MAXIMUM 50 JOBS
    */

    int limit =
    min(51, (int)jsonData.size());

    for(int i = 1; i < limit; i++)
    {
        Job job;

        /*
        TITLE
        */

        if(jsonData[i].contains("position"))
        {
            job.title =
            jsonData[i]["position"];
        }

        /*
        COMPANY
        */

        if(jsonData[i].contains("company"))
        {
            job.company =
            jsonData[i]["company"];
        }

        /*
        CATEGORY
        */

        job.category = "";

        if(jsonData[i].contains("tags"))
        {
            for(auto& tag :
                jsonData[i]["tags"])
            {
                job.category +=
                tag.get<string>() + " ";
            }
        }

        /*
        FIX LINK
        */

        if(jsonData[i].contains("url"))
        {
            string url =
            jsonData[i]["url"].get<string>();

            /*
            FULL URL EXISTS
            */

            if(url.find("http") == 0)
            {
                job.link = url;
            }

            /*
            OTHERWISE ADD PREFIX
            */

            else
            {
                job.link =
                "https://remoteok.com" + url;
            }
        }

        jobs.push_back(job);
    }
}

/*
==================================================
SAVE HTML FILE
==================================================
*/

void saveJobsToHTMLFile()
{
    ofstream html("jobs.html");

    html << "<html>";

    html << "<head>";

    html << "<title>Saved Jobs</title>";

    /*
    STYLE
    */

    html << "<style>";

    html << "body{";
    html << "background:#111827;";
    html << "color:white;";
    html << "font-family:monospace;";
    html << "padding:20px;";
    html << "}";

    html << ".box{";
    html << "border:2px solid #374151;";
    html << "padding:20px;";
    html << "margin-bottom:20px;";
    html << "background:#1f2937;";
    html << "border-radius:10px;";
    html << "}";

    html << "a{";
    html << "color:#22c55e;";
    html << "font-weight:bold;";
    html << "text-decoration:none;";
    html << "}";

    html << "h1{";
    html << "color:#38bdf8;";
    html << "}";

    html << "</style>";

    html << "</head>";

    html << "<body>";

    html << "<h1>=========================================<br>";
    html << "              SAVED JOBS<br>";
    html << "=========================================</h1>";

    int limit =
    min(50, (int)jobs.size());

    for(int i = 0; i < limit; i++)
    {
        html << "<div class='box'>";

        html << "-----------------------------------------<br><br>";

        html << "<b>Job Number :</b> "
             << i + 1
             << "<br><br>";

        html << "<b>Title :</b> "
             << jobs[i].title
             << "<br><br>";

        html << "<b>Company :</b> "
             << jobs[i].company
             << "<br><br>";

        html << "<b>Category :</b> "
             << jobs[i].category
             << "<br><br>";

        /*
        CLICKABLE LINK
        */

        html << "<b>Apply Link :</b> ";

        html << "<a href='"
             << jobs[i].link
             << "' target='_blank'>";

        html << jobs[i].link;

        html << "</a>";

        html << "<br><br>";

        html << "-----------------------------------------";

        html << "</div>";
    }

    html << "</body>";

    html << "</html>";

    html.close();
}

/*
==================================================
OPEN WEBSITE LOOP
==================================================
*/

void openWebsiteLoop()
{
    while(true)
    {
        int number;

        cout << YELLOW
             << "\nEnter Job Number To Open Website"
             << " (0 to stop): "
             << RESET;

        /*
        INPUT VALIDATION
        */

        if(!(cin >> number))
        {
            cin.clear();

            cin.ignore(10000, '\n');

            cout << RED
                 << "\nInvalid Input.\n"
                 << RESET;

            continue;
        }

        /*
        STOP
        */

        if(number == 0)
        {
            break;
        }

        /*
        VALID NUMBER
        */

        if(number >= 1 &&
           number <= jobs.size())
        {
            string command =
            "xdg-open \"" +
            jobs[number - 1].link +
            "\" > /dev/null 2>&1";

            system(command.c_str());

            cout << CYAN
                 << "\nOpening Browser...\n"
                 << RESET;
        }
        else
        {
            cout << RED
                 << "\nInvalid Job Number.\n"
                 << RESET;
        }
    }
}

/*
==================================================
DISPLAY JOBS
==================================================
*/

void displayJobs()
{
    loadJobs();

    saveJobsToHTMLFile();

    cout << MAGENTA
         << "\n========== AVAILABLE JOBS ==========\n"
         << RESET;

    int limit =
    min(50, (int)jobs.size());

    for(int i = 0; i < limit; i++)
    {
        cout << BLUE
             << "\n----------------------------------\n"
             << RESET;

        cout << YELLOW
             << "Job #"
             << i + 1
             << RESET << endl;

        cout << CYAN
             << "Title    : "
             << RESET
             << jobs[i].title << endl;

        cout << CYAN
             << "Company  : "
             << RESET
             << jobs[i].company << endl;

        cout << CYAN
             << "Category : "
             << RESET
             << jobs[i].category << endl;

        cout << MAGENTA
             << "Apply    : "
             << RESET
             << jobs[i].link << endl;

        cout << BLUE
             << "----------------------------------\n"
             << RESET;
    }

    cout << YELLOW
         << "\nJobs saved in jobs.html"
         << "\n(Contains clickable links)\n"
         << RESET;

    openWebsiteLoop();
}

/*
==================================================
SEARCH CATEGORY
==================================================
*/

void searchCategory()
{
    loadJobs();

    saveJobsToHTMLFile();

    string keyword;

    cin.ignore();

    cout << YELLOW
         << "\nEnter Category: "
         << RESET;

    getline(cin, keyword);

    keyword = toLower(keyword);

    bool found = false;

    cout << MAGENTA
         << "\n========== MATCHING JOBS ==========\n"
         << RESET;

    int limit =
    min(50, (int)jobs.size());

    for(int i = 0; i < limit; i++)
    {
        string title =
        toLower(jobs[i].title);

        string category =
        toLower(jobs[i].category);

        bool match = false;

        /*
        DIRECT MATCH
        */

        if(title.find(keyword) != string::npos ||
           category.find(keyword) != string::npos)
        {
            match = true;
        }

        /*
        PYTHON CATEGORY
        */

        else if(keyword == "python")
        {
            if(category.find("python") != string::npos ||
               category.find("django") != string::npos ||
               category.find("flask") != string::npos ||
               title.find("python") != string::npos)
            {
                match = true;
            }
        }

        /*
        ARTS CATEGORY
        */

        else if(keyword == "arts")
        {
            if(category.find("design") != string::npos ||
               category.find("ui") != string::npos ||
               category.find("graphic") != string::npos ||
               category.find("creative") != string::npos)
            {
                match = true;
            }
        }

        /*
        SOFTWARE ENGINEERING
        */

        else if(keyword == "software engineering")
        {
            if(category.find("developer") != string::npos ||
               category.find("backend") != string::npos ||
               category.find("frontend") != string::npos ||
               category.find("full stack") != string::npos)
            {
                match = true;
            }
        }

        /*
        REMOTE
        */

        else if(keyword == "remote")
        {
            match = true;
        }

        /*
        ELECTRICAL ENGINEERING
        */

        else if(keyword == "electrical engineering")
        {
            if(category.find("embedded") != string::npos ||
               category.find("hardware") != string::npos)
            {
                match = true;
            }
        }

        /*
        DISPLAY MATCHED JOBS
        */

        if(match)
        {
            found = true;

            cout << BLUE
                 << "\n----------------------------------\n"
                 << RESET;

            cout << YELLOW
                 << "Job #"
                 << i + 1
                 << RESET << endl;

            cout << CYAN
                 << "Title    : "
                 << RESET
                 << jobs[i].title << endl;

            cout << CYAN
                 << "Company  : "
                 << RESET
                 << jobs[i].company << endl;

            cout << CYAN
                 << "Category : "
                 << RESET
                 << jobs[i].category << endl;

            cout << MAGENTA
                 << "Apply    : "
                 << RESET
                 << jobs[i].link << endl;

            cout << BLUE
                 << "----------------------------------\n"
                 << RESET;
        }
    }

    if(!found)
    {
        cout << RED
             << "\nNo matching jobs found.\n"
             << RESET;
    }
    else
    {
        cout << YELLOW
             << "\nJobs saved in jobs.html"
             << "\n(Contains clickable links)\n"
             << RESET;

        openWebsiteLoop();
    }
}

/*
==================================================
CLEAR FILE
==================================================
*/

void clearFile()
{
    ofstream html("jobs.html",
                  ios::trunc);

    html.close();

    cout << RED
         << "\njobs.html cleared.\n"
         << RESET;
}

/*
==================================================
MAIN FUNCTION
==================================================
*/

int main()
{
    int choice;

    while(true)
    {
        cout << MAGENTA
             << "\n========== JOB SCRAPER ==========\n"
             << RESET;

        cout << CYAN
             << "1. Display Jobs\n"
             << RESET;

        cout << CYAN
             << "2. Search By Category\n"
             << RESET;

        cout << CYAN
             << "3. Clear Saved Jobs File\n"
             << RESET;

        cout << CYAN
             << "4. Exit\n"
             << RESET;

        cout << YELLOW
             << "\nEnter Choice: "
             << RESET;

        /*
        INPUT VALIDATION
        */

        if(!(cin >> choice))
        {
            cin.clear();

            cin.ignore(10000, '\n');

            cout << RED
                 << "\nInvalid Input.\n"
                 << RESET;

            continue;
        }

        if(choice == 1)
        {
            displayJobs();
        }

        else if(choice == 2)
        {
            searchCategory();
        }

        else if(choice == 3)
        {
            clearFile();
        }

        else if(choice == 4)
        {
            cout << RED
                 << "\nExiting Program...\n"
                 << RESET;

            break;
        }

        else
        {
            cout << RED
                 << "\nInvalid Choice.\n"
                 << RESET;
        }
    }

    return 0;
}
