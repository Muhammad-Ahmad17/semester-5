# EdgeMQ: Kafka-Like Message Broker with Multi-Language SDKs

## 🎯 Project Vision

Build a **Kafka-like distributed message broker** that:
- ✅ Runs as a **centralized server** (like Kafka broker)
- ✅ Has **language-agnostic client libraries** (SDKs for Python, Node.js, Go, C, etc.)
- ✅ Supports **pub/sub messaging** with topics
- ✅ Has **persistence** (messages stored on disk)
- ✅ Supports **partitions** (optional, for scaling)
- ✅ Can **sync to cloud** (for backup/analytics)

---

## 📊 Architecture Overview

```
┌─────────────────────────────────────────────────────────────┐
│                     EDGE MESSAGE BROKER                     │
│                     (C Core - Linux/Docker)                 │
│                                                              │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐      │
│  │   Topic 1    │  │   Topic 2    │  │   Topic N    │      │
│  │              │  │              │  │              │      │
│  │ [Partition0] │  │ [Partition0] │  │ [Partition0] │      │
│  │ [Partition1] │  │ [Partition1] │  │ [Partition1] │      │
│  └──────────────┘  └──────────────┘  └──────────────┘      │
│                                                              │
│  Disk: /data/topics/...                                    │
│  In-Memory: Active consumer offsets + session state        │
└─────────────────────────────────────────────────────────────┘
         ▲
         │ Network (TCP/HTTP)
         │
    ┌────┴────┬────────────┬────────────┐
    │          │            │            │
┌───▼──┐  ┌────▼──┐  ┌─────▼──┐  ┌────▼──┐
│Python│  │Node.js│  │  Go    │  │  C    │
│ SDK  │  │ SDK   │  │  SDK   │  │ SDK   │
└──────┘  └───────┘  └────────┘  └───────┘

Apps can use ANY language's SDK to communicate with the broker!
```

---

## 🔧 Core Broker (The Server)

### What it does:

```c
/* Pseudo-code of broker operations */

BROKER_INIT() {
  - Open TCP socket on port 9092 (like Kafka)
  - Create thread pool for handling clients
  - Load topics from disk
  - Start event loop (select/epoll)
}

HANDLE_CLIENT_MESSAGE(msg) {
  - Parse message (protocol: binary or JSON)
  - If PUBLISH -> store in topic partition, notify subscribers
  - If SUBSCRIBE -> add client to topic listeners
  - If FETCH -> send messages to client
  - If COMMIT -> store consumer offset
}

PERSIST_TO_DISK() {
  - Write messages to log files (/data/topics/topic_name/partition_0.log)
  - Write offset commits (/data/offsets/group_name/topic_name_partition_0.offset)
}
```

### Broker Protocol (Binary or JSON):

```json
// PUBLISH message
{
  "command": "publish",
  "topic": "user-events",
  "partition": 0,
  "key": "user123",
  "value": "{...json payload...}",
  "timestamp": 1699699200000
}

// SUBSCRIBE message
{
  "command": "subscribe",
  "topics": ["user-events", "orders"],
  "group_id": "my-consumer-group",
  "from_offset": "latest"  // or "earliest"
}

// FETCH (get messages)
{
  "command": "fetch",
  "topic": "user-events",
  "partition": 0,
  "offset": 100,
  "max_bytes": 1048576
}
```

---

## 📚 Client Libraries (SDKs)

### Why? 

Instead of users writing **complex socket code** to talk to the broker, they use a **simple library API**:

### Example Usage - Same concept, different languages:

#### **Python SDK**
```python
from edgemq import EdgeMQ, Producer, Consumer

# Connect
broker = EdgeMQ("localhost:9092")

# Publish
producer = Producer(broker)
producer.send("user-events", {"user_id": 123, "action": "login"})

# Subscribe
consumer = Consumer(broker, group_id="analytics-group")
consumer.subscribe(["user-events"])

for message in consumer.consume():
    print(f"Topic: {message.topic}, Value: {message.value}")
    consumer.commit()  # Mark as processed
```

#### **Node.js SDK**
```javascript
const { EdgeMQ, Producer, Consumer } = require('@edgemq/sdk');

const broker = new EdgeMQ('localhost:9092');

// Publish
const producer = new Producer(broker);
await producer.send('user-events', { user_id: 123, action: 'login' });

// Subscribe
const consumer = new Consumer(broker, { group_id: 'analytics-group' });
await consumer.subscribe(['user-events']);

consumer.on('message', (msg) => {
  console.log(`Topic: ${msg.topic}, Value:`, msg.value);
  consumer.commit();
});
```

#### **Go SDK**
```go
package main

import "github.com/edgemq/go-sdk"

func main() {
    broker := edgemq.NewBroker("localhost:9092")
    
    // Publish
    producer := edgemq.NewProducer(broker)
    producer.Send("user-events", map[string]interface{}{"user_id": 123})
    
    // Subscribe
    consumer := edgemq.NewConsumer(broker)
    consumer.Subscribe([]string{"user-events"})
    
    for msg := range consumer.Messages {
        fmt.Printf("Topic: %s, Value: %v\n", msg.Topic, msg.Value)
        consumer.Commit()
    }
}
```

#### **C SDK**
```c
#include <edgemq/client.h>

int main() {
    EdgeMQ *broker = edgemq_connect("localhost", 9092);
    
    // Publish
    EdgeMQMessage msg = {
        .topic = "user-events",
        .key = "user123",
        .value = "{\"user_id\": 123}"
    };
    edgemq_publish(broker, &msg);
    
    // Subscribe
    edgemq_subscribe(broker, "my-group", (const char*[]){"user-events"}, 1);
    
    EdgeMQMessage *received;
    while (edgemq_consume(broker, &received) == EDGEMQ_OK) {
        printf("Topic: %s, Value: %s\n", received->topic, received->value);
        edgemq_commit(broker);
    }
    
    edgemq_disconnect(broker);
    return 0;
}
```

---

## 📁 Project Structure

```
EdgeMQ/
│
├── broker/                          # Core broker (C)
│   ├── src/
│   │   ├── main.c                  # Entry point
│   │   ├── broker.c                # Broker logic
│   │   ├── topic.c                 # Topic management
│   │   ├── partition.c             # Partition management
│   │   ├── consumer_group.c        # Consumer group tracking
│   │   ├── network.c               # Socket handling
│   │   ├── persistence.c           # Disk I/O
│   │   └── protocol.c              # Message protocol parsing
│   ├── include/
│   │   ├── broker.h
│   │   ├── message.h
│   │   └── protocol.h
│   ├── Makefile
│   ├── Dockerfile
│   └── edgemq.conf                 # Config (port, paths, etc.)
│
├── sdks/
│   ├── python-sdk/                 # Python client library
│   │   ├── edgemq/
│   │   │   ├── __init__.py
│   │   │   ├── client.py
│   │   │   ├── producer.py
│   │   │   ├── consumer.py
│   │   │   └── protocol.py
│   │   ├── setup.py
│   │   ├── requirements.txt
│   │   └── examples/
│   │
│   ├── nodejs-sdk/                 # Node.js client library
│   │   ├── lib/
│   │   │   ├── index.js
│   │   │   ├── client.js
│   │   │   ├── producer.js
│   │   │   ├── consumer.js
│   │   │   └── protocol.js
│   │   ├── package.json
│   │   ├── Dockerfile
│   │   └── examples/
│   │
│   ├── go-sdk/                     # Go client library
│   │   ├── client.go
│   │   ├── producer.go
│   │   ├── consumer.go
│   │   ├── protocol.go
│   │   ├── go.mod
│   │   └── examples/
│   │
│   └── c-sdk/                      # C client library
│       ├── src/
│       ├── include/
│       ├── Makefile
│       └── examples/
│
├── cloud-sync/                      # Cloud sync service (Node.js)
│   ├── src/
│   │   ├── app.js
│   │   ├── routes/
│   │   ├── models/
│   │   └── middleware/
│   ├── package.json
│   ├── Dockerfile
│   └── docker-compose.yml
│
├── tests/                           # Integration tests
│   ├── test_producer.py
│   ├── test_consumer.py
│   ├── test_broker.c
│   └── docker-compose.test.yml
│
├── docs/
│   ├── README.md                   # Main documentation
│   ├── ARCHITECTURE.md             # Detailed architecture
│   ├── SDK_USAGE.md                # How to use each SDK
│   ├── PROTOCOL.md                 # Wire protocol spec
│   └── DEPLOYMENT.md               # Docker + deployment
│
└── docker-compose.yml              # All services together
```

---

## 🔄 How It Works (Step-by-Step)

### 1️⃣ **Publish Flow**
```
Python App
    │ producer.send("events", data)
    ▼
[Python SDK] → TCP to broker (port 9092)
    │
    ▼ (Binary message: PUBLISH command)
[Broker] → Parse → Find topic "events"
    │
    ▼
[Partition 0] → Append to log file
    │
    ▼ (Notify subscribers)
[All consumers on "events"] ← Message pushed
```

### 2️⃣ **Subscribe/Consume Flow**
```
Go App
    │ consumer.Subscribe("events")
    ▼
[Go SDK] → TCP to broker
    │
    ▼ (Binary message: SUBSCRIBE command)
[Broker] → Register consumer group + topic
    │
    ▼ (Store offset from disk)
[Consumer Group State] ← Tracks last offset
    │
    ▼ (Loop: wait for new messages)
Consumer.Messages ← Messages pushed as they arrive
```

### 3️⃣ **Commit (Offset Tracking)**
```
consumer.commit()
    │
    ▼
[SDK] → Send COMMIT_OFFSET to broker
    │
    ▼
[Broker] → Save offset to disk
    │
    ▼
Next app restart → Consumer resumes from last committed offset
```

---

## 🛣️ Development Phases

### **Phase 1 (Weeks 1-3): Core Broker**
- ✅ Build C broker with TCP socket handling
- ✅ Implement PUBLISH/SUBSCRIBE/FETCH commands
- ✅ Simple in-memory topic + partition storage
- ✅ Disk persistence (append-only log)
- **Deliverable:** Broker runs, accepts TCP connections

### **Phase 2 (Weeks 4-5): Consumer Groups & Offsets**
- ✅ Track consumer groups
- ✅ Store/retrieve offsets
- ✅ Replay messages from offset
- **Deliverable:** Consumer can restart and resume

### **Phase 3 (Weeks 6-7): Python SDK**
- ✅ Create Python wrapper library
- ✅ Simple producer/consumer API
- ✅ Error handling + reconnection
- **Deliverable:** Easy-to-use Python package

### **Phase 4 (Week 8): Node.js SDK**
- ✅ Port to Node.js
- ✅ Promise-based async API
- **Deliverable:** NPM package ready

### **Phase 5 (Week 9): Go + C SDKs**
- ✅ Go client library
- ✅ C client library (for systems programming)
- **Deliverable:** Multi-language support

### **Phase 6 (Weeks 10-11): Cloud Sync**
- ✅ REST API for syncing messages
- ✅ MongoDB storage
- ✅ Dashboard to view messages
- **Deliverable:** Hybrid edge + cloud setup

### **Phase 7 (Weeks 12-14): Polish & Deploy**
- ✅ Docker containers for all services
- ✅ GitHub repo + documentation
- ✅ Performance tests
- ✅ Open source release
- **Deliverable:** Production-ready EdgeMQ

---

## 🎓 Learning Outcomes

By building this, you'll learn:

| Topic | Where | How |
|-------|-------|-----|
| **C Programming** | Broker | Memory management, threading, sockets |
| **Networking** | Broker + SDKs | TCP, protocol design, serialization |
| **Concurrency** | Broker | Mutexes, condition variables, thread pools |
| **Persistence** | Broker | Disk I/O, append-only logs, crash recovery |
| **Design Patterns** | SDKs | Producer/Consumer, connection pooling |
| **Python/Node.js/Go** | SDKs | Language-specific best practices |
| **DevOps** | Cloud + Docker | Containerization, orchestration |
| **Open Source** | Docs + GitHub | README, Contributing guide, versioning |

---

## 🚀 Quick Start (Week 1)

```bash
# 1. Set up broker directory
mkdir -p EdgeMQ/broker/src EdgeMQ/broker/include
cd EdgeMQ/broker

# 2. Write first broker code (socket listener + PUBLISH handler)
# 3. Write simple C client to test
# 4. Verify: client connects → sends PUBLISH → broker receives

# 5. Later: Add Python SDK wrapper on top
```

---

## ✅ Success Criteria

- [ ] Broker accepts multiple TCP connections
- [ ] Can PUBLISH messages to a topic
- [ ] Can SUBSCRIBE and receive messages
- [ ] Messages persisted to disk
- [ ] Consumer offsets tracked
- [ ] At least 3 language SDKs working
- [ ] Docker setup working
- [ ] GitHub repo public with docs
- [ ] Open source ready (MIT/Apache license)

---

## 💡 Why This Approach?

| Traditional | EdgeMQ Approach |
|---|---|
| Learn Kafka externally (passive) | **Build Kafka yourself** (active) |
| Use existing libraries | **Create your own libraries** |
| Generic course project | **Real open-source contribution** |
| Disconnect between theory + practice | **Every concept ties to real code** |

**This is how real engineers learn systems!** 🚀

---

## 📞 Next Steps

1. **Review this architecture** - Does it align with what you want?
2. **Pick a start date** - When do you want to begin Phase 1?
3. **Set up GitHub repo** - Let's initialize the project structure
4. **Week 1 planning** - I'll give you detailed C tasks for broker foundation

Ready? Let's go! 💪
