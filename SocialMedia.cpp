#include <bits/stdc++.h>
using namespace std;

/* ---------------------- Observer Pattern ---------------------- */
class Observer {
public:
    virtual void update(const string& postContent, const string& userId) = 0;
};

/* ---------------------- User Class ---------------------- */
class User : public Observer {
private:
    string userId;
    string username;
    string email;
    set<string> following;
    bool active;

public:
    User(string userId, string username, string email)
        : userId(userId), username(username), email(email), active(true) {}

    void update(const string& postContent, const string& userId) override {
        cout << username << " received a notification: " << userId << " posted \"" << postContent << "\"\n";
    }

    string getUserId() const { return userId; }
    string getUsername() const { return username; }
    void follow(const string& userId) { following.insert(userId); }
    void unfollow(const string& userId) { following.erase(userId); }
};

/* ---------------------- Post Class ---------------------- */
class Post {
private:
    string postId;
    string userId;
    string content;
    time_t timestamp;

public:
    Post() : postId(""), userId(""), content(""), timestamp(time(nullptr)) {}

    Post(string postId, string userId, string content)
        : postId(postId), userId(userId), content(content) {
        timestamp = time(nullptr);
    }

    string getPostId() const { return postId; }
    string getUserId() const { return userId; }
    string getContent() const { return content; }
    time_t getTimestamp() const { return timestamp; }
};

/* ---------------------- Factory Pattern ---------------------- */
class UserFactory {
public:
    static User createUser(string userId, string name, string email) {
        return User(userId, name, email);
    }
};

class PostFactory {
public:
    static Post createPost(string postId, string userId, string content) {
        return Post(postId, userId, content);
    }
};

/* ---------------------- Singleton Pattern: FollowService ---------------------- */
class FollowService {
    unordered_map<string, unordered_set<string>> follow;
    FollowService() {}

public:
    static FollowService& getInstance() {
        static FollowService instance;
        return instance;
    }

    FollowService(const FollowService&) = delete;
    void operator=(const FollowService&) = delete;

    void followUser(string id1, string id2) {
        follow[id1].insert(id2);
    }

    void unFollowUser(string id1, string id2) {
        follow[id1].erase(id2);
    }

    unordered_set<string> getFollowing(string id1) { 
        return follow[id1]; 
    }
};

/* ---------------------- Strategy Pattern: Feed Sorting ---------------------- */
class SortingStrategy {
public:
    virtual void sortFeed(vector<Post>& posts) = 0;
};

class DefaultSorting : public SortingStrategy {
public:
    void sortFeed(vector<Post>& posts) override {
        sort(posts.begin(), posts.end(), [](const Post& a, const Post& b) {
            return a.getTimestamp() > b.getTimestamp();
        });
    }
};

/* ---------------------- SocialNetwork Class ---------------------- */
class SocialNetwork {
    unordered_map<string, Post> post;
    unordered_map<string, User> user;
    unordered_map<string, vector<Post>> PostByUser;
    FollowService& f;
    SortingStrategy* sortingStrategy;

public:
    SocialNetwork(FollowService& followService, SortingStrategy* strategy) 
        : f(followService), sortingStrategy(strategy) {}

    void setSortingStrategy(SortingStrategy* strategy) {
        sortingStrategy = strategy;
    }

    void registerUser(string userId, string name, string email) {
        user.emplace(userId, UserFactory::createUser(userId, name, email));
    }

    void createPost(string id, string userId, string content) {
        if (user.find(userId) == user.end()) {
            cout << "Error: User " << userId << " does not exist!\n";
            return;
        }

        Post p = PostFactory::createPost(id, userId, content);
        post.emplace(id, p);
        PostByUser[userId].push_back(p);

        // Notify followers
        unordered_set<string> followers = f.getFollowing(userId);
        for (const auto& follower : followers) {
            if (user.find(follower) != user.end()) {
                user[follower].update(content, userId);
            }
        }
    }

    vector<Post> getFeedofUser(string id) {
        vector<Post> ans = PostByUser[id]; 

        unordered_set<string> following = f.getFollowing(id);
        for (const auto& it : following) {
            for (const auto& itr : PostByUser[it]) {
                ans.push_back(itr);
            }
        }

        sortingStrategy->sortFeed(ans);

        if (ans.size() > 10) {
            ans.resize(10);
        }

        return ans;
    }
};

/* ---------------------- Main Function ---------------------- */
int main() {
    FollowService& f = FollowService::getInstance();
    DefaultSorting defaultSort;
    SocialNetwork network(f, &defaultSort);

    // Register users
    network.registerUser("1", "Alice", "alice@example.com");
    network.registerUser("2", "Bob", "bob@example.com");
    network.registerUser("3", "Charlie", "charlie@example.com");

    // Follow relationships
    f.followUser("1", "2");
    f.followUser("1", "3");

    // Create posts
    network.createPost("1", "1", "Alice's first post");
    network.createPost("2", "2", "Bob's first post");
    network.createPost("3", "3", "Charlie's first post");
    network.createPost("4", "1", "Alice's second post");

    // Get Alice's feed
    cout << "\nAlice's Feed:\n";
    vector<Post> aliceFeed = network.getFeedofUser("1");
    for (const auto& post : aliceFeed) {
        cout << post.getContent() << " (by " << post.getUserId() << ")\n";
    }

    return 0;
}
