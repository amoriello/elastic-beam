var addon = require('./build/Release/power_ctl');
var pwr_ctl = addon.PowerCtl();

function for_each_outlet(action) {
  for (var i = 1; i <= 4; ++i) {
    action(i);
  }
}


function print_outlet_state(outlet) {
  var state = pwr_ctl.isOn(outlet) ? "On" : "Off";
  console.log("Outlet %s : %s", outlet, state);
}


function print_powerstrip_state() {
  console.log("=========== STATE ===========");
  for_each_outlet(function (outlet) {
    print_outlet_state(outlet);
  });
}


print_powerstrip_state();

for_each_outlet(function (outlet) {
  pwr_ctl.toggle(outlet);
});

print_powerstrip_state();
