#include "ruby.h"

#include "stdafx.h"

#include <stdio.h>
#include "cTLE.h"
#include "cEci.h"
#include "cOrbit.h"
#include "cSite.h"
#include "coord.h"

VALUE SxP4r = Qnil;
VALUE TLE = Qnil;
VALUE Orbit = Qnil;
VALUE ECI = Qnil;
VALUE CoordGeo = Qnil;

static void TLE_free(void *tle)
{
  delete (cTle *)tle;
}

static VALUE TLE_new(VALUE klass, VALUE str1, VALUE str2, VALUE str3) {
  std::string line1 = std::string(RSTRING_PTR(str1));
  std::string line2 = std::string(RSTRING_PTR(str2));
  std::string line3 = std::string(RSTRING_PTR(str3));

  cTle *newTLE = new cTle(line1, line2, line3);

  VALUE rb_TLE = Data_Wrap_Struct(TLE, NULL, TLE_free, newTLE);
  return rb_TLE;
}

static VALUE TLE_get_fractional_day(VALUE self) {
  cTle *TLE_value;
  Data_Get_Struct(self, cTle, TLE_value);

  return rb_float_new(TLE_value->getField(cTle::FLD_EPOCHDAY));
}

static void CoordGeo_free(void *coord_geo) {
  delete (cCoordGeo *)coord_geo;
}

static VALUE CoordGeo_lat(VALUE self) {
  cCoordGeo *coord_geo_value;
  Data_Get_Struct(self, cCoordGeo, coord_geo_value);
  return rb_float_new(coord_geo_value->m_Lat);
}

static VALUE CoordGeo_lon(VALUE self) {
  cCoordGeo *coord_geo_value;
  Data_Get_Struct(self, cCoordGeo, coord_geo_value);
  return rb_float_new(coord_geo_value->m_Lon);
}

static VALUE CoordGeo_alt(VALUE self) {
  cCoordGeo *coord_geo_value;
  Data_Get_Struct(self, cCoordGeo, coord_geo_value);
  return rb_float_new(coord_geo_value->m_Alt);
}

static void ECI_free(void *eci) {
  delete (cEci *)eci;
}

static VALUE ECI_to_geo(VALUE self) {
  cEci *eci_value;
  Data_Get_Struct(self, cEci, eci_value);

  cCoordGeo *geo_value = new cCoordGeo(eci_value->toGeo());

  VALUE rb_CoordGeo = Data_Wrap_Struct(CoordGeo, NULL, CoordGeo_free, geo_value);
  return rb_CoordGeo;
}

static void Orbit_free(void *orbit) {
  delete (cOrbit *)orbit;
}

static VALUE Orbit_new(VALUE klass, VALUE tle) {
  cTle *tle_value;
  Data_Get_Struct(tle, cTle, tle_value);
  cOrbit *newOrbit = new cOrbit(*tle_value);

  VALUE rb_Orbit = Data_Wrap_Struct(Orbit, NULL, Orbit_free, newOrbit);
  return rb_Orbit;
}

static VALUE Orbit_get_position(VALUE self, VALUE time) {
  cOrbit *orbit_value;
  Data_Get_Struct(self, cOrbit, orbit_value);
  
  cEci *eci_value = new cEci;

  orbit_value->getPosition(NUM2DBL(time), eci_value);
  
  VALUE rb_Eci = Data_Wrap_Struct(ECI, NULL, ECI_free, eci_value);
  return rb_Eci;
}

extern "C" void Init_sxp4r() {
  SxP4r = rb_define_module("SxP4r");
  
  TLE = rb_define_class_under(SxP4r, "TLE", rb_cObject);
  rb_define_singleton_method(TLE, "new", (VALUE (*)(...))TLE_new, 3);  
  rb_define_method(TLE, "get_fractional_day", (VALUE (*)(...))TLE_get_fractional_day, 0);

  Orbit = rb_define_class_under(SxP4r, "Orbit", rb_cObject);
  rb_define_singleton_method(Orbit, "new", (VALUE (*)(...))Orbit_new, 1);
  rb_define_method(Orbit, "get_position", (VALUE (*)(...))Orbit_get_position, 1);

  ECI = rb_define_class_under(SxP4r, "ECI", rb_cObject);
  rb_define_method(ECI, "to_geo", (VALUE (*)(...))ECI_to_geo, 0);

  CoordGeo = rb_define_class_under(SxP4r, "CoordGeo", rb_cObject);
  rb_define_method(CoordGeo, "lat", (VALUE (*)(...))CoordGeo_lat, 0);
  rb_define_method(CoordGeo, "lon", (VALUE (*)(...))CoordGeo_lon, 0);
  rb_define_method(CoordGeo, "alt", (VALUE (*)(...))CoordGeo_alt, 0);
}
