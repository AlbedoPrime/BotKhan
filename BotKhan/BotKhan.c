#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>

#define API_URL "https://pt.khanacademy.org/api/v1/assessmentItems"

typedef struct {
    char *body;
    char *type;
} Answer;

typedef struct {
    char *question;
    char *widgets;
} Question;

typedef struct {
    char *answer;
    char *correct;
} Choice;

typedef struct {
    char *value;
    char *correct;
} AnswerForm;

void print_answer(Answer *answer) {
    printf("%s\n", answer->body);
}

void print_image(char *url) {
    // Implementar a lógica para imprimir a imagem
}

void fetch_answer(char *question_id) {
    CURL *curl;
    CURLcode res;
    json_object *json;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        char *url = malloc(strlen(API_URL) + strlen(question_id) + 1);
        sprintf(url, "%s/%s", API_URL, question_id);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFunction, NULL);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "cURL error: %s\n", curl_easy_strerror(res));
        } else {
            json = json_tokener_parse(curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE));
            if(json) {
                json_object *item = json_object_object_get(json, "item");
                if(item) {
                    json_object *question = json_object_object_get(item, "question");
                    if(question) {
                        json_object *widgets = json_object_object_get(question, "widgets");
                        if(widgets) {
                            // Processar os widgets
                            json_object *widget = json_object_array_get(widgets, 0);
                            if(widget) {
                                json_object *options = json_object_object_get(widget, "options");
                                if(options) {
                                    json_object *answers = json_object_object_get(options, "answers");
                                    if(answers) {
                                        // Processar as respostas
                                        json_object *answer = json_object_array_get(answers, 0);
                                        if(answer) {
                                            Answer *a = malloc(sizeof(Answer));
                                            a->body = strdup(json_object_get_string(answer));
                                            a->type = strdup("free_response");
                                            print_answer(a);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

int main() {
    fetch_answer("1234567890");
    return 0;
}
