#include <cmath>
#include <conversions.h>

ConversionFactors::ConversionFactors(const InputParams &ip) {
    _psi_to_pascal = 6894.75729;
    _km_to_m = 1000.0;
    _universal_R = 8314.472;
    _molecular_weight_air = 28.9626;
    _one_atm = 101.325; /* pascal */
    _miles_to_m = 1609.64;
    _inches_to_m = 2.54 / 100.0;
    _gas_R =
        _universal_R / _molecular_weight_air / ip.get_gas_specific_gravity();
    _mmscfd_to_kgps =
        1000.0 * 0.02832 / 86400.0 *
        (_one_atm / (_universal_R * ip.get_temperature()) * 100 * 100 * 100) *
        ip.get_gas_specific_gravity() * _molecular_weight_air;
    _a = std::sqrt(_gas_R * ip.get_temperature());
    _hp_to_watts = 745.7;
};

double ConversionFactors::psi_to_pascal() const { return _psi_to_pascal; };
double ConversionFactors::pascal_to_psi() const {
    return 1.0 / _psi_to_pascal;
};
double ConversionFactors::km_to_m() const { return _km_to_m; };
double ConversionFactors::m_to_km() const { return 1.0 / _km_to_m; };
double ConversionFactors::get_universal_R() const { return _universal_R; };
double ConversionFactors::get_molecular_weight_air() const {
    return _molecular_weight_air;
};
double ConversionFactors::get_one_atm() const { return _one_atm; };
double ConversionFactors::get_gas_R() const { return _gas_R; };
double ConversionFactors::mmscfd_to_kgps() const { return _mmscfd_to_kgps; };
double ConversionFactors::kgps_to_mmscfd() const {
    return 1.0 / _mmscfd_to_kgps;
};
double ConversionFactors::hp_to_watts() const { return _hp_to_watts; };
double ConversionFactors::watts_to_hp() const { return 1.0 / _hp_to_watts; };
double ConversionFactors::get_sound_speed() const { return _a; };
double ConversionFactors::miles_to_m() const { return _miles_to_m; };
double ConversionFactors::m_to_miles() const { return 1.0 / _miles_to_m; };
double ConversionFactors::inches_to_m() const { return _inches_to_m; };
double ConversionFactors::m_to_inches() const { return 1.0 / _inches_to_m; };

ScalingFactors::ScalingFactors(double pressure_scaling, double flow_scaling,
                               double time_scaling, double space_scaling)
    : _pressure_scaling(pressure_scaling), _flow_scaling(flow_scaling),
      _time_scaling(time_scaling), _space_scaling(space_scaling){};

double ScalingFactors::get_pressure_scaling() const {
    return _pressure_scaling;
};
double ScalingFactors::get_flow_scaling() const { return _flow_scaling; };
double ScalingFactors::get_time_scaling() const { return _time_scaling; };
double ScalingFactors::get_space_scaling() const { return _space_scaling; };

ScalingFactors build_scaling_factors(double si_pressure_scaling,
                                     const ConversionFactors &cf) {
    double space_scaling = 1000.0;
    double time_scaling = space_scaling / cf.get_sound_speed();
    double flow_scaling = si_pressure_scaling / cf.get_sound_speed();
    return ScalingFactors(si_pressure_scaling, flow_scaling, time_scaling,
                          space_scaling);
};
