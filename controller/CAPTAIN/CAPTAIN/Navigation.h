#pragma once
#include "INavigation.h"
#include "IGPS.h"
#include "IAutopilot.h"
#include "ITransmitter.h"
#include "TargetPosition.h"
#include "CoverageRectangle.h"
#include "NavigationData.h"

class Navigation :
	public INavigation
{
public:
	Navigation(IGPS&, ITransmitter&, IAutopilot&);
	~Navigation();
	void PerformTask(const Task) override;
	void PerformTask(const Task, const TargetPosition) override;
	void PerformTask(const Task, const CoverageRectangle) override;
	NavigationData GetNavData() override;
	void SetParameters(const float) override;

private:
	IGPS& gps_;
	ITransmitter& transmitter_;
	IAutopilot& auto_pilot_;
	std::vector<Coordinate> completed_path_;
	std::vector<Coordinate> path_;
	int path_start_time_;
	int timestamp_;
	double equitorial_radius;
	double ellipsoid_flattening;
	struct
	{
		float tool_width;
	} parameters_;
	std::vector<Coordinate> calculateP2PPath(const TargetPosition targetposition);
	std::vector<Coordinate> calculateCoverageRectanglePath(const CoverageRectangle coverage_rectangle);
	static std::vector<Coordinate> calculatePathSegments(const Coordinate current_position, const std::vector<Coordinate> key_method_positions, const int method);
	std::vector<Coordinate> calculatePointsFromSegments(const std::vector<Coordinate> path_segments, const int method) const;
	std::tuple<double, std::string, double, int, double, double> calculateTelegramData();
	std::tuple<double, double> calculateRhumbDistanceAndBearing(const Coordinate coordinate_one, const Coordinate coordinate_two) const;
	std::tuple<double, std::string> calculateXte(const Coordinate coordinate_one, const Coordinate coordinate_two) const;
	static std::string appendChecksumToTelegram(std::string telegram);
	int calculateEte() const;
	void setTimestamp();
	void start();
	void continuePath();
	void stop() const;
};
