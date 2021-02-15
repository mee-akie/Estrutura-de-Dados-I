
#include "filaDePrioridade.h"

PFILA criarFila(int max){
    PFILA res = (PFILA) malloc(sizeof(FILADEPRIORIDADE));
    res->maxElementos = max;
    res->arranjo = (PONT*) malloc(sizeof(PONT)*max);
    res->heap = (PONT*) malloc(sizeof(PONT)*max);
    int i;
    for (i=0;i<max;i++) {
        res->arranjo[i] = NULL;
        res->heap[i] = NULL;
    }
    res->elementosNoHeap = 0;
    return res;
}

bool exibirLog(PFILA f){
    printf("Log [elementos: %i]\n", f->elementosNoHeap);
    PONT atual;
    int i;
    for (i=0;i<f->elementosNoHeap;i++){
        atual = f->heap[i];
        printf("[%i;%f;%i] ", atual->id, atual->prioridade, atual->posicao);
    }
    printf("\n\n");
}

int tamanho(PFILA f){

    int tam = f->elementosNoHeap;
    return tam;
}

void reduzirPrioridadeAux2(PFILA f, PONT atual){    //FUNCAO AUXILIAR DE INSERIR
    PONT novo = atual;
    int posicaoPai = (novo->posicao - 1) / 2;
    PONT pai = f->heap[posicaoPai];

    int auxPosHeap = pai->posicao;              //guarda posicao do Pai no arranjo
    pai->posicao = novo->posicao;
    novo->posicao = auxPosHeap;
    f->heap[pai->posicao] = pai;
    f->heap[novo->posicao] = novo;

    posicaoPai = (novo->posicao - 1) / 2;
    pai = f->heap[posicaoPai];

    if (pai->prioridade < novo->prioridade) {
        reduzirPrioridadeAux2(f, novo);
    }
    else{
        return;
    }
}

bool inserirElemento(PFILA f, int id, float prioridade){
    bool res;


    if(id<0 || id >= f->maxElementos){  //id invalido
        res = false;
        return res;
    }

    ///////////FUNCAO AUX - procura id repetido///////////
    int achouID = 0;
    int cont = 0;
    PONT aux = f->heap[cont];

    while(cont < f->elementosNoHeap){
        if(aux->id == id){
            achouID = 1;
            cont = f->elementosNoHeap + 1;
        }
        else{
            cont++;
            aux = f->heap[cont];
        }
    }
    ////////////////////////////////////////////////////

    if(achouID == 1){ //id repetido
        res = false;
        return res;
    }

    else{
        PONT novo = (PONT) malloc(sizeof(PONT));
        novo->prioridade = prioridade;
        novo->id = id;
        novo->posicao = f->elementosNoHeap;
        f->arranjo[novo->id] = novo;          //insere o endereco do elemento no arranjo

        if(tamanho(f) == 0){    //primeiro elemento a ser inserido
            f->heap[0] = novo;
            f->elementosNoHeap = tamanho(f) + 1;    //aumenta o num de elementos validos
            res = true;
            return res;
        }

        else {   //arranjo ja possui elementos validos

            f->elementosNoHeap = tamanho(f) + 1;    //aumenta o num de elementos validos
            f->heap[novo->posicao] = novo;     //insere o endereco do elemento no heap
            int posicaoPai = (novo->posicao - 1) / 2;
            PONT pai = f->heap[posicaoPai];

            if (pai->prioridade < novo->prioridade) {           //ajustar posicoes no arranjo

                reduzirPrioridadeAux2(f, novo);
                res = true;
                return res;
            }

            else{   //nao eh necessario ajustar as posicoes no arranjo
                res = true;
                return res;
            }
        }
    }
}

bool aumentarPrioridade(PFILA f, int id, float novaPrioridade){
    bool res;

    ///////////FUNCAO AUX - verifica existencia do id///////////
    int achouID = 0;
    int cont = 0;
    int guardaPosicao = 0;
    PONT aux = f->heap[cont];

    while(cont < f->elementosNoHeap){
        if(aux->id == id){
            achouID = 1;
            guardaPosicao = aux->posicao;
            cont = f->elementosNoHeap + 1;
        }
        else{
            cont++;
            aux = f->heap[cont];
        }
    }
    ////////////////////////////////////////////////////////////

    if(id<0 || id >= f->maxElementos){  //id invalido
        res = false;
        return res;
    }
    if(achouID == 0){
        res = false;
        return res;
    }
    if(achouID == 1 && f->heap[guardaPosicao]->prioridade >= novaPrioridade) {
        res = false;
        return res;
    }

    else{
        PONT atualiza = f->heap[guardaPosicao];
        atualiza->prioridade = novaPrioridade;      //atualiza valor da prioridade

        int posicaoPai = (atualiza->posicao - 1) / 2;
        PONT pai = f->heap[posicaoPai];

        //VERIFICA SE PRECISA AJUSTAR A POSICAO DOS PAIS:
        if (pai->prioridade < atualiza->prioridade) {

            while(pai->prioridade < atualiza->prioridade){
                int auxPosHeap = pai->posicao;     //guarda a posicao do Pai
                pai->posicao = atualiza->posicao;
                atualiza->posicao = auxPosHeap;
                f->heap[pai->posicao] = pai;
                f->heap[atualiza->posicao] = atualiza;

                if(atualiza->posicao == 0){     //elemento eh a raiz, logo, nao possui pai
                    res = true;
                    return res;
                }
                else {
                    posicaoPai = (atualiza->posicao - 1) / 2;
                    pai = f->heap[posicaoPai];
                }
            }

            res = true;
            return res;
        }

        else{   //nao eh necessario ajustar as posicoes no arranjo
            res = true;
            return res;
        }
    }
}

void reduzirPrioridadeAux(PFILA f, int posicao){

    PONT atualiza = f->heap[posicao];
    int posFilhoEsq = 2 * atualiza->posicao + 1;
    int posFilhoDir = 2 * atualiza->posicao + 2;

    ///////////FUNCAO AUX - verifica existencia de filhos///////////
    int achouEsq = 0;
    int achouDir = 0;

    if(f->heap[posFilhoEsq]) {
        if(posFilhoEsq < f->maxElementos){
            achouEsq = 1;
        }
        else{
            achouEsq = 0;
        }
    }
    if(f->heap[posFilhoDir]){
        if(posFilhoDir < f->maxElementos){
            achouDir = 1;
        }
        else{
            achouDir = 0;
        }
    }

    ////////////////////////////////////////////////////////////

    if(achouDir == 0 && achouEsq == 0){ //elemento eh a folha do heap
        return;
    }

    PONT filhoEsq = f->heap[posFilhoEsq];
    PONT filhoDir = f->heap[posFilhoDir];

    if(f->heap[posFilhoEsq] && !f->heap[posFilhoDir]){ //so existe o filho da Esquerda

        if(atualiza->prioridade < filhoEsq->prioridade){
            int auxPosHeap = atualiza->posicao;
            atualiza->posicao = filhoEsq->posicao;
            filhoEsq->posicao = auxPosHeap;
            f->heap[atualiza->posicao] = atualiza;
            f->heap[filhoEsq->posicao] = filhoEsq;
            return;
        }
        return;
    }

    if(f->heap[posFilhoDir] && !f->heap[posFilhoEsq]){ //so existe o filho da Direita
        if(atualiza->prioridade < filhoDir->prioridade){
            int auxPosHeap = atualiza->posicao;
            atualiza->posicao = filhoDir->posicao;
            filhoDir->posicao = auxPosHeap;
            f->heap[atualiza->posicao] = atualiza;
            f->heap[filhoDir->posicao] = filhoDir;
            return;
        }
        return;
    }

    //POSSUI OS FILHOS DA DIREITA E ESQUERDA:

    if(filhoEsq->prioridade > filhoDir->prioridade && filhoEsq->prioridade > atualiza->prioridade){
        int auxPosHeap = atualiza->posicao;
        atualiza->posicao = filhoEsq->posicao;
        filhoEsq->posicao = auxPosHeap;
        f->heap[atualiza->posicao] = atualiza;
        f->heap[filhoEsq->posicao] = filhoEsq;

        reduzirPrioridadeAux(f, atualiza->posicao); //chamada recursiva
    }
    else if(filhoDir->prioridade > filhoEsq->prioridade && filhoDir->prioridade > atualiza->prioridade){
        int auxPosHeap = atualiza->posicao;
        atualiza->posicao = filhoDir->posicao;
        filhoDir->posicao = auxPosHeap;
        f->heap[atualiza->posicao] = atualiza;
        f->heap[filhoDir->posicao] = filhoDir;

        reduzirPrioridadeAux(f, atualiza->posicao); //chamada recursiva
    }
    else{  //ambos os filhos tem prioridades menores
        return;
    }
}



bool reduzirPrioridade(PFILA f, int id, float novaPrioridade){
    bool res;

    ///////////FUNCAO AUX - verifica existencia do id///////////
    int achouID = 0;
    int cont = 0;
    int guardaPosicao = 0;
    PONT aux = f->heap[cont];

    while(cont < f->elementosNoHeap){
        if(aux->id == id){
            achouID = 1;
            guardaPosicao = cont;
            cont = f->elementosNoHeap + 1;
        }
        else{
            cont++;
            aux = f->heap[cont];
        }
    }
    ////////////////////////////////////////////////////////////

    if(id<0 || id >= f->maxElementos){  //id invalido
        res = false;
        return res;
    }
    if(achouID == 0){
        res = false;
        return res;
    }
    if(achouID == 1 && f->heap[guardaPosicao]->prioridade <= novaPrioridade){
        res = false;
        return res;
    }

    else{
        PONT atualiza = f->heap[guardaPosicao];
        atualiza->prioridade = novaPrioridade;      //atualiza valor da prioridade

        int posFilhoEsq = 2 * atualiza->posicao + 1;
        int posFilhoDir = 2 * atualiza->posicao + 2;

        ///////////FUNCAO AUX - verifica existencia de filhos///////////
        int achouEsq = 0;
        int achouDir = 0;

        if(f->heap[posFilhoEsq]) {
            achouEsq = 1;
        }
        if(f->heap[posFilhoDir]){
            achouDir = 1;
        }
        ////////////////////////////////////////////////////////////

        if(achouDir == 0 && achouEsq == 0){ //elemento eh a folha do heap
            res = true;
            return res;
        }

        PONT filhoEsq = f->heap[posFilhoEsq];
        PONT filhoDir = f->heap[posFilhoDir];

        if(f->heap[posFilhoEsq] && !f->heap[posFilhoDir]){ //so existe o filho da Esquerda
            if(atualiza->prioridade < filhoEsq->prioridade){
                int auxPosHeap = atualiza->posicao;
                atualiza->posicao = filhoEsq->posicao;
                filhoEsq->posicao = auxPosHeap;
                f->heap[atualiza->posicao] = atualiza;
                f->heap[filhoEsq->posicao] = filhoEsq;
                return true;
            }
            return true;
        }

        if(f->heap[posFilhoDir] && !f->heap[posFilhoEsq]){ //so existe o filho da Direita
            if(atualiza->prioridade < filhoDir->prioridade){
                int auxPosHeap = atualiza->posicao;
                atualiza->posicao = filhoDir->posicao;
                filhoDir->posicao = auxPosHeap;
                f->heap[atualiza->posicao] = atualiza;
                f->heap[filhoDir->posicao] = filhoDir;
                return true;
            }
            return true;
        }

        //POSSUI OS FILHOS DA DIREITA E ESQUERDA:

        if(filhoEsq->prioridade > filhoDir->prioridade && filhoEsq->prioridade > atualiza->prioridade){
            reduzirPrioridadeAux(f, atualiza->posicao);
            return true;
        }
        else if(filhoDir->prioridade > filhoEsq->prioridade && filhoDir->prioridade > atualiza->prioridade){
            reduzirPrioridadeAux(f, atualiza->posicao);
            return true;
        }
        else{  //ambos os filhos tem prioridades menores
            return true;
        }
    }
}

void refazHeapMaximo(PFILA f, PONT atual){

    PONT atualiza = atual;
    int posFilhoEsq = 2 * atualiza->posicao + 1;
    int posFilhoDir = 2 * atualiza->posicao + 2;

    ///////////FUNCAO AUX - verifica existencia de filhos///////////
    int achouEsq = 0;
    int achouDir = 0;

    if(f->heap[posFilhoEsq]) {
        if(posFilhoEsq < f->maxElementos){
            achouEsq = 1;
        }
        else{
            achouEsq = 0;
        }
    }
    if(f->heap[posFilhoDir]){
        if(posFilhoDir < f->maxElementos){
            achouDir = 1;
        }
        else{
            achouDir = 0;
        }
    }
    ////////////////////////////////////////////////////////////

    if(achouDir == 0 && achouEsq == 0){ //elemento eh a folha do heap
        return;
    }

    PONT filhoEsq = f->heap[posFilhoEsq];
    PONT filhoDir = f->heap[posFilhoDir];

    if(f->heap[posFilhoEsq] && !f->heap[posFilhoDir]){ //so existe o filho da Esquerda
        if(atualiza->prioridade < filhoEsq->prioridade){
            int auxPosHeap = atualiza->posicao;
            atualiza->posicao = filhoEsq->posicao;
            filhoEsq->posicao = auxPosHeap;
            f->heap[atualiza->posicao] = atualiza;
            f->heap[filhoEsq->posicao] = filhoEsq;
            return;
        }
        return;
    }

    if(f->heap[posFilhoDir] && !f->heap[posFilhoEsq]){ //so existe o filho da Direita
        if(atualiza->prioridade < filhoDir->prioridade){
            int auxPosHeap = atualiza->posicao;
            atualiza->posicao = filhoDir->posicao;
            filhoDir->posicao = auxPosHeap;
            f->heap[atualiza->posicao] = atualiza;
            f->heap[filhoDir->posicao] = filhoDir;
            return;
        }
        return;
    }

    //POSSUI OS FILHOS DA DIREITA E ESQUERDA:

    if(filhoEsq->prioridade > filhoDir->prioridade && filhoEsq->prioridade > atualiza->prioridade){
        int auxPosHeap = atualiza->posicao;
        atualiza->posicao = filhoEsq->posicao;
        filhoEsq->posicao = auxPosHeap;
        f->heap[atualiza->posicao] = atualiza;
        f->heap[filhoEsq->posicao] = filhoEsq;

        refazHeapMaximo(f, atualiza); //chamada recursiva
    }
    else if(filhoDir->prioridade > filhoEsq->prioridade && filhoDir->prioridade > atualiza->prioridade){
        int auxPosHeap = atualiza->posicao;
        atualiza->posicao = filhoDir->posicao;
        filhoDir->posicao = auxPosHeap;
        f->heap[atualiza->posicao] = atualiza;
        f->heap[filhoDir->posicao] = filhoDir;

        refazHeapMaximo(f, atualiza); //chamada recursiva
    }
    else{  //ambos os filhos tem prioridades menores
        return;
    }
}



PONT removerElemento(PFILA f){
    int cont = 0;
    PONT atual = f->heap[cont];
    int elemValidos = 0;

    while (atual && cont < f->elementosNoHeap) {
        elemValidos++;
        cont++;
        atual = f->heap[cont];
    }

    if(elemValidos == 0){   //fila esta vazia
        return NULL;
    }

    if(elemValidos == 1){   //so ha a Raiz no heap
        PONT raizAntiga = f->heap[0];

        for (int i=0;i<f->maxElementos;i++) { //reinicializa a fila
            f->arranjo[i] = NULL;
            f->heap[i] = NULL;
        }

        f->elementosNoHeap = 0;
        return raizAntiga;
    }

    else {
        PONT raizAntiga = f->heap[0];
        PONT raizNova = f->heap[tamanho(f)-1];

        f->elementosNoHeap = tamanho(f) - 1;  //reajusta o numero de elementos do heap

        int auxPosHeap = raizNova->posicao;
        raizNova->posicao = raizAntiga->posicao;
        raizAntiga->posicao = auxPosHeap;
        f->heap[raizNova->posicao] = raizNova;
        f->heap[raizAntiga->posicao] = raizAntiga;
        int guardaID = raizAntiga->id;
        f->arranjo[guardaID] = NULL;
        f->heap[raizAntiga->posicao] = NULL;
        raizNova->posicao = 0;

        //REORGANIZANDO O HEAP:
        int posFilhoEsq = 2 * raizNova->posicao + 1;
        int posFilhoDir = 2 * raizNova->posicao + 2;

        ///////////FUNCAO AUX - verifica existencia de filhos///////////
        int achouEsq = 0;
        int achouDir = 0;

        if(f->heap[posFilhoEsq]) {
            if(posFilhoEsq < f->maxElementos){
                achouEsq = 1;
            }
            else{
                achouEsq = 0;
            }
        }
        if(f->heap[posFilhoDir]){
            if(posFilhoDir < f->maxElementos){
                achouDir = 1;
            }
            else{
                achouDir = 0;
            }
        }
        ////////////////////////////////////////////////////////////

        if (achouDir == 0 && achouEsq == 0) { //nao ha filhos, so existe um elemento no arranjo (raiz)
            return raizAntiga;
        }

        PONT filhoEsq = f->heap[posFilhoEsq];
        PONT filhoDir = f->heap[posFilhoDir];

        if (f->heap[posFilhoEsq] && !f->heap[posFilhoDir]) { //so existe o filho da Esquerda
            if (raizNova->prioridade < filhoEsq->prioridade) {
                int auxPosHeap = raizNova->posicao;
                raizNova->posicao = filhoEsq->posicao;
                filhoEsq->posicao = auxPosHeap;
                f->heap[raizNova->posicao] = raizNova;
                f->heap[filhoEsq->posicao] = filhoEsq;
                return raizAntiga;
            }
            return raizAntiga;
        }

        if (f->heap[posFilhoDir] && !f->heap[posFilhoEsq]) { //so existe o filho da Direita
            if (raizNova->prioridade < filhoDir->prioridade) {
                int auxPosHeap = raizNova->posicao;
                raizNova->posicao = filhoDir->posicao;
                filhoDir->posicao = auxPosHeap;
                f->heap[raizNova->posicao] = raizNova;
                f->heap[filhoDir->posicao] = filhoDir;
                return raizAntiga;
            }
            return raizAntiga;
        }

        //POSSUI OS FILHOS DA DIREITA E ESQUERDA:

        if (filhoEsq->prioridade > filhoDir->prioridade && filhoEsq->prioridade > raizNova->prioridade) {
            refazHeapMaximo(f, raizNova);
            return raizAntiga;
        }
        else if (filhoDir->prioridade > filhoEsq->prioridade && filhoDir->prioridade > raizNova->prioridade) {
            refazHeapMaximo(f, raizNova);
            return raizAntiga;
        }
        else {  //ambos os filhos tem prioridades menores
            return raizAntiga;
        }
    }
}



bool consultarPrioridade(PFILA f, int id, float* resposta){
    bool res;
    ///////////FUNCAO AUX - verifica existencia do id///////////
    int cont = 0;
    int achouID = 0;
    int guardaPosicao = 0;
    PONT aux = f->heap[cont];

    while(cont < f->elementosNoHeap){
        if(aux->id == id){
            guardaPosicao = aux->posicao;
            achouID = 1;
            cont = f->elementosNoHeap + 1;
        }
        else{
            cont++;
            aux = f->heap[cont];
        }
    }
    ////////////////////////////////////////////////////////////

    if(id<0 || id>=f->maxElementos){    //id invalido
        res = false;
        return res;
    }
    if(achouID == 0){
        res = false;
        return res;
    }
    else{
        *resposta = f->heap[guardaPosicao]->prioridade;
        res=true;
        return res;
    }
}