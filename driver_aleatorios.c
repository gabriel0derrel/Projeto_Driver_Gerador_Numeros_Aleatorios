#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/random.h>
#include <linux/prandom.h>
#include <linux/jiffies.h>

#define NOME "Num_Aleatorios"

static int id_driver;
static u32 seed = 0;
static u32 numero_aleatorio = 0;
static char buffer_de_escrita_temp[32];
struct rnd_state estado_aleatorio;

static int abrir_driver(struct inode *inode, struct file *arquivo_aberto){
    return 0;
}

static ssize_t ler_driver(struct file *arquivo, char __user *buffer_usuario, size_t tamanho_maximo, loff_t *deslocamento){
    size_t num_bytes_copiados = min(sizeof(u32), tamanho_maximo);
    
    numero_aleatorio = prandom_u32_state(&estado_aleatorio);

    // coloca o número no buffer do usuário
    if(copy_to_user(buffer_usuario, &numero_aleatorio, num_bytes_copiados)){ // sucesso -> retorna 0
        return -EFAULT;
    }
    
    return num_bytes_copiados;
}

static ssize_t escrever_no_driver(struct file *arquivo, const char __user *buffer_usuario, size_t tamanho_lido, loff_t *deslocamento){
    if(tamanho_lido == 0){
        return -EINVAL; // erro
    }

    if(tamanho_lido >= sizeof(buffer_de_escrita_temp)){
        return -EINVAL; // erro
    }
    
    // copia a string do buffer do usuário
    if(copy_from_user(buffer_de_escrita_temp, buffer_usuario, tamanho_lido)){ // sucesso -> retorna 0
        return -EFAULT;
    }
    
    buffer_de_escrita_temp[tamanho_lido] = '\0'; // torna o vetor de bytes em uma string
    
    if(kstrtou32(buffer_de_escrita_temp, 0, &seed)){ // str to int do kernel
        return -EINVAL;
    }
    prandom_seed_state(&estado_aleatorio, seed); // seta o estado aleatório do prandom com a seed
    
    return tamanho_lido;
}

// operações do driver
static struct file_operations operacoes = {
    .owner = THIS_MODULE,
    .open = abrir_driver,
    .read = ler_driver,
    .write = escrever_no_driver,
};

static int __init inicializar(void){
    id_driver = register_chrdev(0, NOME, &operacoes); // 0 -> id dinamico
    if(id_driver < 0){
        return id_driver;
    }

    seed = (u32) jiffies;
    prandom_seed_state(&estado_aleatorio, seed); // seta o estado aleatório do prandom com a seed
    
    // cria o driver
    device_create(NULL, NULL, MKDEV(id_driver, 0), NULL, NOME);
    
    return 0;
}

static void __exit sair(void){
    // destroi o driver
    device_destroy(NULL, MKDEV(id_driver, 0));
    unregister_chrdev(id_driver, NOME); // retira o degistro do /dev
}

module_init(inicializar); // seta função de inicialização
module_exit(sair); // seta função de fechamento

MODULE_LICENSE("GPL"); // define a licença, para poder utilizar os metodos do kernel