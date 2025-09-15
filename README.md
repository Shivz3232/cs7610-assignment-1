# Student

- Name: Shivu Donmardi Gorva
- NUID: 002599393

# Assignment

- Course: CS 7610
- Assignment #: 1

# How to run

The app can be executed exactly as mentioned in the assignment description using the commands:
1. `docker build . -t prj1`
2. `docker compose up`

However, since the provided docker compose expects dockerfile to have an ENTRYPOINT instead of CMD, I was unable to use `make` to dynamically read the configuration parameters from `.env` file. Thus, running the above two commands will defautl the configuration parameters as mentioned in the next section.

Note: Switching to the commented dockerfile line `CMD` will start the app using make, loading the `.env`, but will fail as docker compose imposes the `-h hostsfile.txt` command.

**IMPORTANT: to run the app without modifying any dockerfiles but with complete logs enabled please change the `cEnv` value in `src/config/config.c` to `development`**

# Configuration Parameters

There are 6 configuration parameters.
- C_ENV
- PORT
- HOSTSFILE_PATH
- MAX_PEER_NAME_SIZE
- MAX_PEERS
- MAX_MESSAGE_SIZE

Their value is read from the environment variables.

If environment variables aren't set, they default to the following values:
- C_ENV: `production`
- PORT: `3000`
- HOSTSFILE_PATH: `hostsfile.txt`
- MAX_PEER_NAME_SIZE: `100`
- MAX_PEERS: `100`
- MAX_MESSAGE_SIZE: `100`
