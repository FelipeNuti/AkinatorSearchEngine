#include <bits/stdc++.h>
using namespace std;

#define DEBUG 0

class Akinator
{
    private:
        int num_nodes = 0;
        vector<string> questions;
        vector<vector<pair<string, int>>> pAnsNode;
        string clientName;
        string accountName;
        int num_articles = 0;
    
    public:
        Akinator(vector<string> articles, string rquestion, vector<string> ranswers, vector<int> rnodes)
        {
            this->set_node(rquestion, ranswers, rnodes);
            //this->run_node(0);

            for (auto article : articles)
            {
                this->set_article(article);
                this->num_articles += 1;
            }
            
        }
        int set_node(string question, vector<string> answers, vector<int> nodes)
        {
            if (answers.size() != nodes.size())
            {
                if (DEBUG) printf("Error when setting new node");
                return 1;
            }

            int node_num = questions.size();
            vector<pair<string, int>> my_vec;

            for (int i = 0; i < answers.size(); i++)
            {
                pair<string, int> p = {answers.at(i), nodes.at(i)};
                my_vec.push_back(p);
            }

            this->pAnsNode.push_back(my_vec);
            this->questions.push_back(question);
            if (DEBUG) printf("\033[1;35mNode\033[0m with number \033[1;35m%d\033[0m sucessfully created\n", node_num);
            this->num_nodes += 1;
            return 0;
        }

        int set_article(string link)
        {
            vector<string> answers = {""};
            vector<int> nodes = {-1};
            if (DEBUG) printf("\033[1;32m(A) \033[0m");
            return this->set_node(link, answers, nodes);
        }

        void run_node(int node, int * new_node)
        {
            printf("\033[1;35m\n%s\033[0m \n", this->questions.at(node).c_str());
            int index = 1;
            for (auto ans : this->pAnsNode.at(node))
            {
                string ansText = ans.first;
                printf("\t%d: %s\n", index, ansText.c_str());
                index += 1;
            }
            printf("\033[1;35mAnswer number: \033[0m");
            int response;
            scanf("%d", &response);
            response--;
            *new_node = (this->pAnsNode.at(node).at(response).second);
        }

        int run_article(int node)
        {
            printf("\n\033[1;35mThis article may have the answer to your problem:\n\n\033[0m");
            printf("%s\n\n", this->questions.at(node).c_str());
            return 0;
        }

        bool isArticle(int node)
        {
            return (this->pAnsNode.at(node).size() == 1 
                    || this->pAnsNode.at(node).at(0).second == -1);
        }

        int file_ticket(vector<int> log, int yn)
        {
            FILE *fp;
            string fileName = "ticket_logs.csv";
            char arr[fileName.size()+1];
            strcpy(arr, fileName.c_str());
            if (!(fp = fopen(arr, "w")))
            {
                printf("Erro ao abrir o arquivo %s\n", fileName.c_str());
                return 1;
            }

            fprintf(fp, "AccountName,ClientName,NodePath,Answered\n");
            fprintf(fp, "%s,%s,'", this->accountName.c_str(), this->clientName.c_str());
            for (int i = 0; i < log.size(); i++)
            {
                fprintf(fp, "%d", log.at(i));
                if (i != log.size()-1)
                {
                    fprintf(fp, ",");
                }
            }

            fprintf(fp, "',%d", yn);
            return 0;
        }

        bool handleEOG()
        {
            char yn;
            printf("\033[1;35mDid this article answer your query? (Y/N) \033[0m");
            scanf("%c", &yn);
            scanf("%c", &yn);
            if (yn == 'Y')
            {
                printf("Thank you! Have a good day!\n");
                return true;
            }

            else if (yn == 'N')
            {
                printf("Don't worry, we've got this! Feel free to open a ticket detailing your issues in our form:\n https://help-tickets.vtex.com/pt/support\n");
                return false;
            }
        }

        int play()
        {
            //this->run_node(0);
            printf("Account name: ");
            cin >> this->accountName;
            printf("Client name: ");
            cin >> this->clientName;

            int node = 0;
            vector<int> log;
            log.push_back(node);

            bool answered = false;
            bool finished = false;

            while (!finished)
            {
                this->run_node(node, &node);
                log.push_back(node);
                if (node <= this->num_articles && node > 0)
                {
                    this->run_article(node);
                    char yn;
                    answered = this->handleEOG();
                    finished = true;
                }
            }
            
            this->file_ticket(log, int(answered));
            return int(answered);
        }

};

int main()
{   
    ios_base::sync_with_stdio(true);
    string question = "In what area is your problem?";
    vector<string> answers = {"Subscriptions", "Payments"};
    vector<int> nodes = {7, 8};

    vector<string> articles = 
    {
        "https://help.vtex.com/tutorial/como-configurar-assinaturas--1FA9dfE7vJqxBna9Nft5Sj",
        "https://help.vtex.com/tutorial/como-funciona-a-assinatura--frequentlyAskedQuestions_4453",
        "https://help.vtex.com/tutorial/como-migrar-da-recorrencia-v1-para-assinaturas-v2--3nJ4pbsRHEyKmvoN0foYRi",
        "https://help.vtex.com/tutorial/onde-vejo-a-data-de-captura-do-pagamento--7DD0JK5cbYC2UkmCYgy8I4",
        "https://help.vtex.com/tutorial/como-funciona-a-retentativa-do-gateway-de-pagamentos--2JaHsjQIs0ceks0e2Mkwgu",
        "https://help.vtex.com/tutorial/como-achar-nsu-e-tid-do-pedido--frequentlyAskedQuestions_477",
    };

    Akinator* akinator = new Akinator(articles, question, answers, nodes);

    string question1 = "What were you trying to do when this problem occured?";
    vector<string> answers1 = 
    {
        "Setting up signatures in my store", 
        "Learning how signatures work", 
        "Migrating from an older product to VTEX Subscriptions"
    };

    vector<int> nodes1 = {1, 2, 3};

    if (akinator->set_node(question1, answers1, nodes1)) return 0;

    string question2 = "In what area is your problem?";
    vector<string> answers2 = 
    {
        "Seeing the date of a payment", 
        "Learning how to use VTEX to never miss a payment again", 
        "Seeing a payment's identification numbers"
    };

    vector<int>  nodes2 = {4, 5, 6};

    if (akinator->set_node(question2, answers2, nodes2)) return 0;

    akinator->play();

    return 0;
    
}