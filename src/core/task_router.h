#pragma once

#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <future>
#include <atomic>

namespace mcp_tool {
namespace core {

enum class TaskStatus {
    PENDING,
    RUNNING,
    COMPLETED,
    FAILED,
    CANCELLED
};

struct Task {
    std::string id;
    std::string name;
    std::function<bool()> execute;
    TaskStatus status;
    std::string error_message;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point started_at;
    std::chrono::system_clock::time_point completed_at;
};

struct DockerContainer {
    std::string name;
    std::string image;
    std::vector<std::string> command;
    std::map<std::string, std::string> volumes;
    std::map<std::string, std::string> environment;
    bool health_check_enabled;
    std::string health_check_command;
};

class TaskRouter {
public:
    TaskRouter(size_t max_threads = std::thread::hardware_concurrency());
    ~TaskRouter();

    // Task management
    std::string SubmitTask(const std::string& name, std::function<bool()> task_func);
    bool CancelTask(const std::string& task_id);
    TaskStatus GetTaskStatus(const std::string& task_id) const;
    std::string GetTaskError(const std::string& task_id) const;

    // Docker container orchestration
    bool StartContainer(const DockerContainer& container);
    bool StopContainer(const std::string& container_name);
    bool IsContainerHealthy(const std::string& container_name);
    std::vector<std::string> GetRunningContainers() const;

    // Pipeline orchestration
    bool ExecuteTilesetPipeline(const std::string& input_path, const std::string& output_path);
    bool ExecuteMapPipeline(const std::string& tileset_path, const std::string& map_config_path, 
                           const std::string& output_path);

    // Thread management
    void Start();
    void Stop();
    void WaitForAllTasks();

    // Health monitoring
    void StartHealthMonitoring();
    void StopHealthMonitoring();

private:
    std::vector<std::thread> worker_threads_;
    std::queue<std::shared_ptr<Task>> task_queue_;
    std::unordered_map<std::string, std::shared_ptr<Task>> tasks_;
    
    mutable std::mutex queue_mutex_;
    mutable std::mutex tasks_mutex_;
    std::condition_variable task_condition_;
    
    std::atomic<bool> stop_requested_;
    std::atomic<bool> running_;
    
    size_t max_threads_;
    std::string shared_volume_path_;
    
    // Docker management
    std::unordered_map<std::string, DockerContainer> active_containers_;
    std::thread health_monitor_thread_;
    std::atomic<bool> health_monitoring_active_;

    // Worker thread function
    void WorkerThread();
    
    // Docker helper functions
    bool ExecuteDockerCommand(const std::vector<std::string>& command, std::string& output);
    bool BuildDockerCommand(const DockerContainer& container, std::vector<std::string>& command);
    void MonitorContainerHealth();
    
    // Pipeline helper functions
    std::shared_ptr<Task> CreateSpriteToolTask(const std::string& input_path, const std::string& output_path);
    std::shared_ptr<Task> CreateTileCheckerTask(const std::string& tileset_path);
    std::shared_ptr<Task> CreateMapMakerTask(const std::string& tileset_path, 
                                           const std::string& map_config_path, 
                                           const std::string& output_path);
    
    // Utility functions
    std::string GenerateTaskId();
    void LogTaskEvent(const std::string& task_id, const std::string& event, const std::string& details = "");
    void WriteAuditLog(const std::string& entry);
};

} // namespace core
} // namespace mcp_tool
