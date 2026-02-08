# EykBorder

Include de Criação de Bordas e Detecção de Áreas de Alta Performance para SA-MP.
Crie bordas visuais (GangZones) com detecção de entrada/saída otimizada.

## Como usar

1. Use as funções:
```pawn
new border = Border_Create(minx, miny, maxx, maxy); // Criar borda
Border_Show(playerid, border, 0xFF0000AA); // Mostrar para jogador
Border_ShowForAll(border, 0xFF0000AA); // Mostrar para todos
IsPlayerInBorder(playerid, border); // Verificar se está dentro
```

2. Callbacks:
```pawn
public OnPlayerEnterEykBorder(playerid, border_id) { ... }
public OnPlayerLeaveEykBorder(playerid, border_id) { ... }
```

## Configuração

- `#define MAX_BORDERS 100` (Padrão)
- `#define BORDER_DEFAULT_SIZE 9.0` (Espessura padrão)

Dev: Marinho!
