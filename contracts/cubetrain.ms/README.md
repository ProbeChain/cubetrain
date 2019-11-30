cubetrain.ms
--------

Actions:
The naming convention is codeaccount::actionname followed by a list of paramters.

Create a proposal
## cubetrain.ms::propose    proposer proposal_name requested trx
   - **proposer** account proposing a transaction
   - **proposal_name** name of the proposal (should be unique for proposer)
   - **requested** permission levels expected to approve the proposal
   - **trx** proposed transaction

   Storage changes are billed to 'proposer'

Approve a proposal
## cubetrain.ms::approve    proposer proposal_name level
   - **proposer** account proposing a transaction
   - **proposal_name** name of the proposal
   - **level** permission level approving the transaction

   Storage changes are billed to 'proposer'

Revoke an approval of transaction
## cubetrain.ms::unapprove    proposer proposal_name level
   - **proposer** account proposing a transaction
   - **proposal_name** name of the proposal
   - **level** permission level revoking approval from the transaction

   Storage changes are billed to 'proposer'

Cancel a proposal
## cubetrain.ms::cancel    proposer proposal_name canceler
   - **proposer** account proposing a transaction
   - **proposal_name** name of the proposal
   - **canceler** account canceling the transaction (only proposer can cancel not expired transaction)

Execute a proposal
## cubetrain.ms::exec    proposer proposal_name executer
   - **proposer** account proposing a transaction
   - **proposal_name** name of the proposal
   - **executer** account executing the transaction


Clseat usage example.

Prerequisites:
   - cubetrain.tk contract installed to cubetrain.tk account, cubetrain.ms contract installed on cubetrain.ms account which is a privileged account.
   - account 'treasury' is the issuer of SEAT token.
   - account 'tester' exists.
   - keys to accounts 'treasury' and 'tester' imported into local wallet, the wallet is unlocked.

One user creates a proposal:
````
$ cliseat multisig propose test '[{"actor": "treasury", "permission": "active"}]' '[{"actor": "treasury", "permission": "active"}]' cubetrain.tk issue '{"to": "tester", "quantity": "1000.0000 SEAT", "memo": ""}' -p tester
executed transaction: e26f3a3a7cba524a7b15a0b6c77c7daa73d3ba9bf84e83f9c2cdf27fcb183d61  336 bytes  107520 cycles
#    cubetrain.ms <= cubetrain.ms::propose          {"proposer":"tester","proposal_name":"test","requested":[{"actor":"treasury","permission":"active"}]...
````

Another user reviews the transaction:
````
$ cliseat multisig review tester test -p treasury
{
  "proposal_name": "test",
  "requested_approvals": [{
      "actor": "treasury",
      "permission": "active"
    }
  ],
  "provided_approvals": [],
  "packed_transaction": "00aee75a0000000000000000000000000100a6823403ea30550000000000a5317601000000fe6a6cd4cd00000000a8ed323219000000005c95b1ca809698000000000004454f530000000000",
  "transaction": {
    "expiration": "2018-05-01T00:00:00",
    "region": 0,
    "ref_block_num": 0,
    "ref_block_prefix": 0,
    "max_net_usage_words": 0,
    "max_kcpu_usage": 0,
    "delay_sec": 0,
    "context_free_actions": [],
    "actions": [{
        "account": "cubetrain.tk",
        "name": "issue",
        "authorization": [{
            "actor": "treasury",
            "permission": "active"
          }
        ],
        "data": {
          "to": "tester",
          "quantity": "1000.0000 SEAT",
          "memo": ""
        },
        "hex_data": "000000005c95b1ca809698000000000004454f530000000000"
      }
    ]
  }
}
````

And then approves it:
````
$ cliseat multisig approve tester test '{"actor": "treasury", "permission": "active"}' -p treasury
executed transaction: 475970a4b0016368d0503d1ce01577376f91f5a5ba63dd4353683bd95101b88d  256 bytes  108544 cycles
#    cubetrain.ms <= cubetrain.ms::approve          {"proposer":"tester","proposal_name":"test","level":{"actor":"treasury","permission":"active"}}
````

First user initiates execution:
````
$ cliseat multisig exec tester test -p tester
executed transaction: 64e5eaceb77362694055f572ae35876111e87b637a55250de315b1b55e56d6c2  248 bytes  109568 cycles
#    cubetrain.ms <= cubetrain.ms::exec             {"proposer":"tester","proposal_name":"test","executer":"tester"}
````
