# 📄 PS2 File Generator

A program developed in **C (Imperative Programming context)** that generates **.PS2 banking files** used for **direct debit and batch payment operations in Portugal**.

This project was created as part of an academic assignment for the *Imperative Programming* course.

---

## 📌 Overview

The **PS2 format** is a standardized file format used by Portuguese banks to process **batch financial transactions**, such as:

- Direct debits  
- Salary payments  
- Invoice collections  
- Bulk transfers  

A PS2 file is composed of structured records that allow banks to automatically process multiple transactions at once.  

This application automates the creation of those files while ensuring **data validation and integrity**.

---

## ⚙️ Features

- ✅ Generation of valid `.PS2` files  
- ✅ Support for multiple transaction records  
- ✅ Automatic calculation of totals and control fields  
- ✅ Validation of:
  - **NIB (Número de Identificação Bancária)**  
  - **NIF (Número de Identificação Fiscal)**  
- ✅ Input processing and structured output generation  
- ✅ Error detection for invalid data  

---

## 🧩 PS2 File Structure

A PS2 file follows a strict structure composed of **fixed-length records (80 bytes)** organized into three main sections:

### 1. Header (Record Type 1)

Contains:
- Ordering account (NIB)  
- Processing date  
- Operation type  
- Currency (EUR)

### 2. Movements (Record Type 2)

One record per transaction:
- Destination account (NIB)  
- Amount  
- References  

### 3. Footer (Record Type 9)

Contains:
- Total number of transactions  
- Total amount  
- Control values  

---

## 🔍 Validation Algorithms

Two key validation mechanisms are implemented in this project:

### 🏦 NIB Validation

The NIB (Portuguese bank account number) includes **control digits** used to detect errors.

- Based on checksum logic (modular arithmetic)
- Ensures integrity of banking data
- Prevents invalid or mistyped account numbers  

---

### 🧾 NIF Validation

The NIF (Portuguese Tax Identification Number) is validated using:

- A **check digit algorithm**
- Weighted sums of digits
- Verification against the final control digit  

The system can:
- Confirm if a NIF is valid  
- Reject incorrect values before file generation  

---

## 🎓 Academic Context

This project was developed as part of the **Imperative Programming** course within a **Computer Systems Engineering degree**.

It aims to apply core programming concepts such as:

- Structured programming in C  
- File manipulation (input/output)  
- Implementation of validation algorithms  
- Data formatting according to real-world specifications  
- Problem-solving in a financial/banking context  

The entire project was developed in **Portuguese**, following the requirements defined in the course assignment.

---

## 📜 License

This project is provided for **educational purposes only**.

You are free to:
- Use the code for learning and academic reference  
- Study and adapt the implementation  

However:
- It is not intended for production use in financial systems  
- No guarantees are provided regarding correctness, security, or compliance  

---

## 👤 Author

**Henrique Santos Pereira**  
Computer Systems Engineering Student  

GitHub: https://github.com/hpereira07
