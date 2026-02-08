# EykBorder

Include de Criação de Bordas e Detecção de Áreas de Alta Performance para SA-MP.
Crie bordas visuais (GangZones) com detecção de entrada/saída otimizada (O(1)) usando Streamer Plugin.

## Como usar

1. Use as funções:
```pawn
new border_id = Border_Create(minx, miny, maxx, maxy, size); // Criar borda
Border_Destroy(border_id); // Destruir borda

Border_Show(playerid, border_id, color); // Mostrar para jogador
Border_Hide(playerid, border_id); // Esconder de jogador

Border_ShowForAll(border_id, color); // Mostrar para todos
Border_HideForAll(border_id); // Esconder de todos

IsPlayerInBorder(playerid, border_id); // Verificar se está dentro
```

2. Callbacks automáticas:
```pawn
public OnPlayerEnterEykBorder(playerid, border_id) { ... }
public OnPlayerLeaveEykBorder(playerid, border_id) { ... }
```

## Configuração

- `#define MAX_BORDERS 100` (Padrão)
- `#define BORDER_DEFAULT_SIZE 9.0` (Espessura padrão)

Pronto para usar!
