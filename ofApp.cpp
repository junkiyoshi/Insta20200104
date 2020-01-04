#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofEnableDepthTest();

	this->frame_mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face_mesh.clear();
	this->frame_mesh.clear();
	this->planet_list.clear();

	int radius_span = 5;
	int deg_span = 10;
	int radius = 100;
	int range = 500;

	if (ofGetFrameNum() % 60 < 45) {

		this->noise_seed += ofMap(ofGetFrameNum() % 60, 0, 45, 0.015, 0);
	}

	for (int i = 0; i < 5; i++) {

		this->planet_list.push_back(glm::vec2(
			ofMap(ofNoise(ofRandom(1000), this->noise_seed), 0, 1, -range, range),
			ofMap(ofNoise(ofRandom(1000), this->noise_seed), 0, 1, -range, range)
		));
	}
	
	for (int deg_start = 0; deg_start < 360; deg_start += deg_span) {

		auto location = glm::vec2(radius * cos((deg_start + deg_span * 0.5) * DEG_TO_RAD), radius * sin((deg_start + deg_span * 0.5) * DEG_TO_RAD));
		auto z = 0;
		for (auto& planet : this->planet_list) {

			auto distance = glm::distance(location, planet) * 0.75;
			if (distance < 150) {

				z -= distance;
			}
			else if (distance < 300) {

				z += distance - 150;
			}
		}

		int param_index = this->face_mesh.getNumVertices();
		for (int deg = deg_start; deg < deg_start + deg_span; deg++) {

			int index = this->face_mesh.getNumVertices();

			vector<glm::vec3> vertices;

			vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), z));
			vertices.push_back(glm::vec3((radius + radius_span) * cos(deg * DEG_TO_RAD), (radius + radius_span) * sin(deg * DEG_TO_RAD), z));
			vertices.push_back(glm::vec3((radius + radius_span) * cos((deg + 1) * DEG_TO_RAD), (radius + radius_span) * sin((deg + 1) * DEG_TO_RAD), z));
			vertices.push_back(glm::vec3(radius * cos((deg + 1) * DEG_TO_RAD), radius * sin((deg + 1) * DEG_TO_RAD), z));

			vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), 0));
			vertices.push_back(glm::vec3((radius + radius_span) * cos(deg * DEG_TO_RAD), (radius + radius_span) * sin(deg * DEG_TO_RAD), 0));
			vertices.push_back(glm::vec3((radius + radius_span) * cos((deg + 1) * DEG_TO_RAD), (radius + radius_span) * sin((deg + 1) * DEG_TO_RAD), 0));
			vertices.push_back(glm::vec3(radius * cos((deg + 1) * DEG_TO_RAD), radius * sin((deg + 1) * DEG_TO_RAD), 0));

			this->face_mesh.addVertices(vertices);

			this->face_mesh.addIndex(index + 0);	this->face_mesh.addIndex(index + 1); this->face_mesh.addIndex(index + 2);
			this->face_mesh.addIndex(index + 0); this->face_mesh.addIndex(index + 2); this->face_mesh.addIndex(index + 3);

			this->face_mesh.addIndex(index + 4);	this->face_mesh.addIndex(index + 5); this->face_mesh.addIndex(index + 6);
			this->face_mesh.addIndex(index + 4); this->face_mesh.addIndex(index + 6); this->face_mesh.addIndex(index + 7);

			this->face_mesh.addIndex(index + 0);	this->face_mesh.addIndex(index + 3); this->face_mesh.addIndex(index + 7);
			this->face_mesh.addIndex(index + 0); this->face_mesh.addIndex(index + 4); this->face_mesh.addIndex(index + 7);

			this->face_mesh.addIndex(index + 1);	this->face_mesh.addIndex(index + 2); this->face_mesh.addIndex(index + 6);
			this->face_mesh.addIndex(index + 1);	this->face_mesh.addIndex(index + 5); this->face_mesh.addIndex(index + 6);

			this->frame_mesh.addVertices(vertices);

			this->frame_mesh.addIndex(index + 0); this->frame_mesh.addIndex(index + 3);
			this->frame_mesh.addIndex(index + 4); this->frame_mesh.addIndex(index + 7);

			this->frame_mesh.addIndex(index + 1); this->frame_mesh.addIndex(index + 2);
			this->frame_mesh.addIndex(index + 5); this->frame_mesh.addIndex(index + 6);
		}

		this->face_mesh.addIndex(param_index + 0); this->face_mesh.addIndex(param_index + 1); this->face_mesh.addIndex(param_index + 5);
		this->face_mesh.addIndex(param_index + 0); this->face_mesh.addIndex(param_index + 4); this->face_mesh.addIndex(param_index + 5);

		this->frame_mesh.addIndex(param_index + 0); this->frame_mesh.addIndex(param_index + 1);
		this->frame_mesh.addIndex(param_index + 4); this->frame_mesh.addIndex(param_index + 5);

		this->frame_mesh.addIndex(param_index + 0); this->frame_mesh.addIndex(param_index + 4);
		this->frame_mesh.addIndex(param_index + 1); this->frame_mesh.addIndex(param_index + 5);

		param_index = this->face_mesh.getNumVertices() - 8;

		this->face_mesh.addIndex(param_index + 2); this->face_mesh.addIndex(param_index + 3); this->face_mesh.addIndex(param_index + 6);
		this->face_mesh.addIndex(param_index + 3); this->face_mesh.addIndex(param_index + 6); this->face_mesh.addIndex(param_index + 7);

		this->frame_mesh.addIndex(param_index + 2); this->frame_mesh.addIndex(param_index + 3);
		this->frame_mesh.addIndex(param_index + 6); this->frame_mesh.addIndex(param_index + 7);

		this->frame_mesh.addIndex(param_index + 2); this->frame_mesh.addIndex(param_index + 6);
		this->frame_mesh.addIndex(param_index + 3); this->frame_mesh.addIndex(param_index + 7);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	
	ofSetColor(39);
	this->face_mesh.drawFaces();

	for (auto& planet : this->planet_list) {

		ofDrawSphere(glm::vec3(planet, 0), 10);
	}

	for (int i = 0; i < this->planet_list.size() - 1; i++) {

		ofDrawLine(this->planet_list[i], this->planet_list[i + 1]);
	}
	ofDrawLine(this->planet_list[this->planet_list.size() - 1], this->planet_list[0]);

	ofSetColor(239);
	this->frame_mesh.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}