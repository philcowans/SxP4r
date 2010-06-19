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
VALUE Site = Qnil;
VALUE CoordTopo = Qnil;

static void CoordTopo_free(void *coord_topo) {
  delete (cCoordTopo *)coord_topo;
}

static VALUE CoordTopo_az(VALUE self) {
  cCoordTopo *coord_topo_value;
  Data_Get_Struct(self, cCoordTopo, coord_topo_value);
  return rb_float_new(coord_topo_value->m_Az);
}

static VALUE CoordTopo_el(VALUE self) {
  cCoordTopo *coord_topo_value;
  Data_Get_Struct(self, cCoordTopo, coord_topo_value);
  return rb_float_new(coord_topo_value->m_El);
}

static VALUE CoordTopo_range(VALUE self) {
  cCoordTopo *coord_topo_value;
  Data_Get_Struct(self, cCoordTopo, coord_topo_value);
  return rb_float_new(coord_topo_value->m_Range);
}


static void Site_free(void *orbit) {
  delete (cSite *)orbit;
}

static VALUE Site_get_look_angle(VALUE self, VALUE eci) {
  cEci *eci_value;
  Data_Get_Struct(eci, cEci, eci_value);
  
  cSite *site_value;
  Data_Get_Struct(self, cSite, site_value);

  cCoordTopo *coord_value = new cCoordTopo(site_value->getLookAngle(*eci_value));
  VALUE rb_coord = Data_Wrap_Struct(CoordTopo, NULL, CoordTopo_free, coord_value);

  return rb_coord;
}

static VALUE Site_new(VALUE klass, VALUE lat, VALUE lon, VALUE alt) {
  cSite *newSite = new cSite(NUM2DBL(lat), NUM2DBL(lon), NUM2DBL(alt));

  VALUE rb_Site = Data_Wrap_Struct(Site, NULL, Site_free, newSite);
  return rb_Site;
}


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

static VALUE ECI_x(VALUE self) {
  cEci *eci_value;
  Data_Get_Struct(self, cEci, eci_value);

  return rb_float_new(eci_value->getPos().m_x);
}

static VALUE ECI_y(VALUE self) {
  cEci *eci_value;
  Data_Get_Struct(self, cEci, eci_value);

  return rb_float_new(eci_value->getPos().m_y);
}

static VALUE ECI_z(VALUE self) {
  cEci *eci_value;
  Data_Get_Struct(self, cEci, eci_value);

  return rb_float_new(eci_value->getPos().m_z);
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
  rb_define_method(ECI, "x", (VALUE (*)(...))ECI_x, 0);
  rb_define_method(ECI, "y", (VALUE (*)(...))ECI_y, 0);
  rb_define_method(ECI, "z", (VALUE (*)(...))ECI_z, 0);

  CoordGeo = rb_define_class_under(SxP4r, "CoordGeo", rb_cObject);
  rb_define_method(CoordGeo, "lat", (VALUE (*)(...))CoordGeo_lat, 0);
  rb_define_method(CoordGeo, "lon", (VALUE (*)(...))CoordGeo_lon, 0);
  rb_define_method(CoordGeo, "alt", (VALUE (*)(...))CoordGeo_alt, 0);

  Site = rb_define_class_under(SxP4r, "Site", rb_cObject);
  rb_define_singleton_method(Site, "new", (VALUE (*)(...))Site_new, 3);
  rb_define_method(Site, "get_look_angle", (VALUE (*)(...))Site_get_look_angle, 1);

  CoordTopo = rb_define_class_under(SxP4r, "CoordTopo", rb_cObject);
  rb_define_method(CoordTopo, "az", (VALUE (*)(...))CoordTopo_az, 0);
  rb_define_method(CoordTopo, "el", (VALUE (*)(...))CoordTopo_el, 0);
  rb_define_method(CoordTopo, "range", (VALUE (*)(...))CoordTopo_range, 0);

}
