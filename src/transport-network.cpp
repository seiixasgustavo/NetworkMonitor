#include <NetworkMonitor/transport-network.h>
#include <algorithm>
#include <memory>
#include <vector>

using NetworkMonitor::TransportNetwork;

std::shared_ptr<TransportNetwork::GraphNode>
TransportNetwork::GetStation(const Id &id) const {
  auto stationIt{stations_.find(id)};
  return (stationIt == stations_.end()) ? nullptr : stationIt->second;
}

bool TransportNetwork::AddStation(const Station &station) {
  if (GetStation(station.id) != nullptr) {
    return false;
  }

  auto node{
      std::make_shared<GraphNode>(GraphNode{station.id, station.name, 0, {}})};
  stations_.emplace(station.id, std::move(node));

  return true;
}
