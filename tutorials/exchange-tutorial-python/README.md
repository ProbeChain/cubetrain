The following steps must be taken for the example script to work.

0. Create wallet
0. Create account for cubetrain.tk
0. Create account for scott
0. Create account for exchange
0. Set token contract on cubetrain.tk
0. Create SEAT token
0. Issue initial tokens to scott

**Note**:
Deleting the `transactions.txt` file will prevent replay from working.


### Create wallet
`cliseat wallet create`

### Create account steps
`cliseat create key`

`cliseat create key`

`cliseat wallet import  --private-key <private key from step 1>`

`cliseat wallet import  --private-key <private key from step 2>`

`cliseat create account cubetrain <account_name> <public key from step 1> <public key from step 2>`

### Set contract steps
`cliseat set contract cubetrain.tk /contracts/cubetrain.tk -p cubetrain.tk@active`

### Create SEAT token steps
`cliseat push action cubetrain.tk create '{"issuer": "cubetrain.tk", "maximum_supply": "100000.0000 SEAT", "can_freeze": 1, "can_recall": 1, "can_whitelist": 1}' -p cubetrain.tk@active`

### Issue token steps
`cliseat push action cubetrain.tk issue '{"to": "scott", "quantity": "900.0000 SEAT", "memo": "testing"}' -p cubetrain.tk@active`
