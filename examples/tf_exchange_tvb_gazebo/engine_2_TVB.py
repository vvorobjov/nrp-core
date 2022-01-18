"""Python Engine 1. Will get current engine time and make it accessible as a datapack"""

from nrp_core.engines.python_json import EngineScript,RegisterEngine
#from tvb.simulator.lab import *

import sys
sys.path.append("/home/aaron/nrp-core/examples/tf_exchange_tvb/")
from tvb_interface import *
#import tvb_interface
import logging

logging.basicConfig(level=logging.DEBUG)

@RegisterEngine()
class Script(EngineScript):
	def initialize(self):
		#"""Initialize datapack1 with time"""
		print("Engine 1 is initializing. Registering datapack...")
		
		#TVB:
		self._registerDataPack("lpg")
		self.model = "OscillatorT" #T
		self.tvb = regularRun(sim_length=100, g=1.0, s=1.0, dt=0.1, period=1, omega=60, 
					filename='connectivity_68.zip', modelExec=self.model)

		self._registerDataPack("actors")
		self._setDataPack("actors", {})
		
		#{ "time" : self._time.count(), "timestep": 0 }
		
		self.time_serie = np.array([])

	def runLoop(self, asd): # timestep
		"""Update datapack1 at every timestep"""
		
		print("Engine 1 in process..")
		
		""" Process camera feedback as input for TVB"""
		# Color recognition  as TVB input
		#try:
		#fb_left = self._getDataPack("lpg").data["rate"]
		#print("---fb_left:",fb_left)
		#except Exception as e:
		#	print(e) 
		#fb_left = self._getDataPack("lpg").data["rate"]
		#print("---fb_left:",fb_left)
		# not red color then random walk, if yes then more value  more left-right ( mul factor)
		# 
		# Nest tvb
		# tvb multiscale
		
		#TVB:		
		timestep, loop_data = self.tvb.simulate_python(sim_length=50,logger=logging)

		#Gazebo movement
		left = loop_data[-1,0,1,0]
		right = loop_data[-1,0,2,0]
		go = loop_data[-1,0,3,0]
		
		dic_move={"left":left,"right":right,"go":go}

		
		self.time_serie = np.concatenate([self.time_serie, timestep])
		
		self._setDataPack("actors", {"move": dic_move})
		"""
		{ "time" : self._time.count(), 
						"timestep": timestep.shape , 
						"Total": self.time_serie.shape,
						"move": dic_move})
		"""
		print("Engine 1 sending: " + str(self._getDataPack("actors")))
        
		

	def shutdown(self):
		print("Engine 1 is shutting down")
        
