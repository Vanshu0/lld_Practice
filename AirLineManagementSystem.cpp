#include <bits/stdc++.h>
using namespace std;

enum class SeatStatus { Empty, Booked };
enum class BookingStatus { Confirmed, Cancelled };

class Flight;
class Passenger;
class Booking;
class Baggage;

class Baggage {
    int baggageNo;
    int weight;

public:
    Baggage(int baggageNo, int weight) : baggageNo(baggageNo), weight(weight) {}
};

class Seat {
    int price;
    int seatNo;
    SeatStatus status;
    Passenger* passenger;
    Baggage* baggage;

public:
    Seat(int price, int seatNo) : price(price), seatNo(seatNo), status(SeatStatus::Empty), passenger(nullptr), baggage(nullptr) {}

    bool isAvailable() const {
        return status == SeatStatus::Empty;
    }

    void assign(Passenger* p, Baggage* b) {
        passenger = p;
        baggage = b;
        status = SeatStatus::Booked;
    }

    void release() {
        passenger = nullptr;
        baggage = nullptr;
        status = SeatStatus::Empty;
    }

    int getPrice() const { return price; }
    int getSeatNo() const { return seatNo; }
};

class Booking {
    Passenger* passenger;
    Flight* flight;
    Seat* seat;
    Baggage* baggage;
    BookingStatus status;

public:
    Booking(Passenger* p, Flight* f, Seat* s, Baggage* b)
        : passenger(p), flight(f), seat(s), baggage(b), status(BookingStatus::Confirmed) {}

    void cancel() {
        status = BookingStatus::Cancelled;
        seat->release();
    }

    BookingStatus getStatus() const { return status; }
};

class Flight {
    string source, destination, date;
    int flightNo;
    unordered_map<int, Seat*> seats;

public:
    Flight(string src, string dest, string dt, int no)
        : source(src), destination(dest), date(dt), flightNo(no) {}

    string getSource() const { return source; }
    string getDestination() const { return destination; }
    string getDate() const { return date; }

    void addSeat(int seatNo, int price) {
        seats[seatNo] = new Seat(price, seatNo);
    }

    Seat* getSeat(int seatNo) {
        return seats.count(seatNo) ? seats[seatNo] : nullptr;
    }

    vector<Seat*> getAvailableSeats() {
        vector<Seat*> available;
        for (auto& pair : seats) {
            if (pair.second->isAvailable()) {
                available.push_back(pair.second);
            }
        }
        return available;
    }
};

class Passenger {
    string name;
    vector<Booking*> bookings;

public:
    Passenger(string name) : name(name) {}

    void addBooking(Booking* booking) {
        bookings.push_back(booking);
    }
};

class BookingService {
public:
    bool bookSeat(Flight* flight, Passenger* passenger, int seatNo, Baggage* baggage) {
        Seat* seat = flight->getSeat(seatNo);
        if (!seat || !seat->isAvailable()) {
            cout << "Seat not available\n";
            return false;
        }
        seat->assign(passenger, baggage);
        Booking* booking = new Booking(passenger, flight, seat, baggage);
        passenger->addBooking(booking);
        cout << "Seat booked successfully\n";
        return true;
    }
};

class AirlineManagementSystem {
    unordered_map<int, Flight*> flights;

public:
    void addFlight(string src, string dest, string date, int flightNo) {
        flights[flightNo] = new Flight(src, dest, date, flightNo);
    }

    void addSeatToFlight(int flightNo, int seatNo, int price) {
        if (flights.count(flightNo)) {
            flights[flightNo]->addSeat(seatNo, price);
        }
    }

    vector<Flight*> searchFlights(string src, string dest, string date) {
        vector<Flight*> result;
        for (auto& pair : flights) {
            Flight* flight = pair.second;
            if (flight->getSource() == src && flight->getDestination() == dest && flight->getDate() == date) {
                result.push_back(flight);
            }
        }
        return result;
    }

    Flight* getFlight(int flightNo) {
        return flights.count(flightNo) ? flights[flightNo] : nullptr;
    }
};

int main() {
    AirlineManagementSystem system;
    BookingService bookingService;

    system.addFlight("Delhi", "Mumbai", "2025-05-01", 101);
    system.addSeatToFlight(101, 1, 5000);
    system.addSeatToFlight(101, 2, 5500);

    Passenger* passenger = new Passenger("Aryan Tyagi");
    Baggage* baggage = new Baggage(1, 15);

    Flight* flight = system.getFlight(101);
    if (flight) {
        bookingService.bookSeat(flight, passenger, 1, baggage);
    }

    return 0;
}
