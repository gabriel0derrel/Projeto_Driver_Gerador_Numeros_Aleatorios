# Driver de Dispositivo de Números Aleatórios

Este projeto implementa um driver de dispositivo para Linux que fornece números aleatórios através do sistema de arquivos `/dev`.

## Pré-requisitos

- Sistema Linux com kernel compilável
- Ferramentas de compilação (make, gcc)
- Headers do kernel (kernel-headers)

## Instalação e Uso

### 1. Compilar o driver
```bash
make
```

### 2. Carregar o driver
```bash
sudo make install
```

### 3. Verificar instalação
```bash
cat /proc/devices
```

### 4. Testar o driver

**Setar a seed no driver (Write):**
```bash
echo "12345" > /dev/Num_Aleatorios
```

**Ler um valor aleatório do driver (Read):**
```bash
hexdump -n 4 -e '"0x%08X \n"' /dev/Num_Aleatorios
```

### 5. Remover o driver
```bash
sudo make remove
```

### 6. Limpar arquivos de compilação
```bash
make clean
```

## Descrição

O driver cria um driver de dispositivo de caractere em `/dev/Num_Aleatorios` que permite:
- Setar seeds
- Ler valores aleatórios de 4 bytes

O formato de leitura usa `hexdump` para exibir o valor em hexadecimal de 32 bits (0xXXXXXXXX).

## Estrutura do Projeto

- `Makefile` - Script de compilação e instalação
- `*.c` - Código fonte do driver (não especificado no texto fornecido)

## Notas de Segurança

- O comando `make install` e `make remove` requerem privilégios de superusuário (sudo)
- O dispositivo `/dev/Num_Aleatorios` pode requerer ajustes de permissão para acesso de usuários não-root

## Licença

Veja o arquivo [LICENSE](./LICENSE) para informações referentes ao licenciamento.