#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>

namespace NetworkMonitor
{

    using Id = std::string;

    struct Station
    {
        Id id{};
        std::string name{};

        bool operator==(const Station &other) const;
    };

    struct Route
    {
        Id id{};
        std::string direction{};
        Id lineId{};
        Id startStationId{};
        Id endStationId{};
        std::vector<Id> stops{};

        bool operator==(const Route &other) const;
    };

    struct Line
    {
        Id id{};
        std::string name{};
        std::vector<Route> routes{};

        bool operator==(const Line &other) const;
    };

    struct PassengerEvent
    {
        enum class Type
        {
            In,
            Out
        };

        Id stationId{};
        Type type{Type::In};
    };

    class TransportNetwork
    {
    public:
        TransportNetwork();
        ~TransportNetwork();

        TransportNetwork(const TransportNetwork &copied);

        TransportNetwork(TransportNetwork &&moved);

        TransportNetwork &operator=(const TransportNetwork &copied);

        TransportNetwork &operator=(TransportNetwork &&moved);

        bool AddStation(const Station &station);

        bool AddLine(const Line &line);

        bool RecordPassengerEvent(const PassengerEvent &event);

        long long int GetPassengerCount(const Id &station) const;

        std::vector<Id> GetRoutesServingStation(const Id &station) const;

        bool SetTravelTime(const Id &stationA, const Id &stationB,
                           const unsigned int travelTime);

        unsigned int GetTravelTime(const Id &stationA, const Id &stationB) const;

        unsigned int GetTravelTime(const Id &line, const Id &route,
                                   const Id &stationA, const Id &stationB) const;

        bool FromJson(nlohmann::json &&src);

    private:
        struct GraphNode;
        struct GraphEdge;

        struct RouteInternal;
        struct LineInternal;

        struct GraphNode
        {
            Id stationId{};
            std::string name{};
            long long int passengerCount{0};
            std::vector<std::shared_ptr<GraphEdge>> edges{};

            std::vector<std::shared_ptr<GraphEdge>>::const_iterator
            FindEdgeForRoute(const std::shared_ptr<RouteInternal> &route) const;
        };

        struct GraphEdge
        {
            std::shared_ptr<RouteInternal> route{nullptr};
            std::shared_ptr<GraphNode> nextStop{nullptr};
            unsigned int travelTime{0};
        };

        struct RouteInternal
        {
            Id id{};
            std::shared_ptr<LineInternal> line{nullptr};
            std::vector<std::shared_ptr<GraphNode>> stops{};
        };

        struct LineInternal
        {
            Id id{};
            std::string name{};
            std::unordered_map<Id, std::shared_ptr<RouteInternal>> routes{};
        };

        std::unordered_map<Id, std::shared_ptr<GraphNode>> stations_;
        std::unordered_map<Id, std::shared_ptr<LineInternal>> lines_;

        std::shared_ptr<GraphNode> GetStation(const Id &stationId) const;

        std::shared_ptr<LineInternal> GetLine(const Id &lineId) const;

        std::shared_ptr<RouteInternal> GetRoute(const Id &lineId,
                                                const Id &routeId) const;

        bool AddRouteToLine(const Route &route,
                            const std::shared_ptr<LineInternal> &LineInternal);
    };

} // namespace NetworkMonitor
