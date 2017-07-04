# Nelson Victor Cruz Hdez
# SOM implementation for user behavior obtention pattern

# References
# [1] http://www.r-tutor.com/r-introduction/data-frame/data-import
# [2] https://www.slideshare.net/shanelynn/2014-0117-dublin-r-selforganising-maps-for-customer-segmentation-shane-lynn
# [3] http://www.shanelynn.ie/self-organising-maps-for-customer-segmentation-using-r/
# [4] https://en.wikibooks.org/wiki/Data_Mining_Algorithms_In_R/Clustering/Self-Organizing_Maps_(SOM)#Introduction
# [5] https://stackoverflow.com/questions/9109778/random-sampling-matrix
# [6] http://www.r-tutor.com/r-introduction/list
# [7] https://www.rdocumentation.org/packages/kohonen/versions/2.0.19/topics/som
# [8] https://www.rdocumentation.org/packages/kohonen/versions/2.0.19/topics/plot.kohonen
# [9] https://cran.r-project.org/web/packages/kohonen/kohonen.pdf
# [10] http://www.statmethods.net/input/exportingdata.html
# [11] https://stackoverflow.com/questions/10608526/writing-a-matrix-to-a-file-without-a-header-and-row-numbers

# === Global variables ===
source('coolBlueHotRed.R')

working_directory <- "./Documents/git/UserBehaviorAnalysisUsingSOM/R"
output_file_folder <- "./r_trained_som"
dataset_file_folder <- "./chunks"
train_dataset_max_size <- 100000
epochs <- 10
learning_rate <- c(0.05, 0.01)

#[circular(euclidean distance) | square(maximum distances)]
neighbourhood <- "circular"

#[rectangular | hexagonal]
topography <- "rectangular"

# === Train module ===
# Needed package
require(kohonen)

setup_environment(working_directory)

users <- c(4, 5, 9, 10)
lattice_dimensions <- c(50, 75, 100, 125)

get_trained_som(users, lattice_dimensions,
                output_file_folder, dataset_file_folder,
                train_dataset_max_size, epochs,
                learning_rate, neighbourhood,topography)


# === Obtained information analysis ===
# Datasets es un lista con 3 vectores build, train y evaluate

# Forma 1 de acceder a los vectores
datasets[[1]] # build
datasets[[2]] # train
datasets[[3]] # evaluate

# Forma 2 de acceder a los vectores
datasets$build
datasets$train
datasets$evaluate

summary(datasets)

plot(som_model, type="changes")
plot(som_model, type="count")
plot(som_model, type="dist.neighbours")
plot(som_model, type="codes")

summary(som_model)
summary(som_model$data)
summary(som_model$unit.classif)
summary(som_model$distances)
summary(som_model$grid)

# som_model$codes have the trained info of the SOM lattic
typeof(som_model$codes)
som_model$codes
summary(som_model$codes)
length(som_model$codes[[1]][1,])
som_model$codes[[1]][1,]
...
...
som_model$codes[[1]][2500,]

summary(som_model$changes)
summary(som_model$alpha)
summary(som_model$radius)
summary(som_model$user.weights)
summary(som_model$distance.weights)
summary(som_model$maxNA.fraction)
summary(som_model$whatmap)


plot(som_model, type = "property",
     property = som_model$codes[[1]][,3],
     main=colnames(som_model$codes)[[1]][,3],
     palette.name=coolBlueHotRed)


# ============= Funtion declarations =============
setup_environment <- function(working_directory){
  getwd()
  setwd(working_directory)
  getwd()
}

get_datasets <- function(build_file, train_file, evaluate_file, max_elements){
  # Build dataset
  import_build_dataset <- read.table(build_file)
  length(import_build_dataset[,1])
  
  # Train dataset
  import_train_dataset <- read.table(train_file)
  total_import_train_dataset <- length(import_train_dataset[,1])
  
  # If training dataset length is bigger than training_elements_subset, a subset is obtained.
  if(total_import_train_dataset > max_elements){
    train_samples <- sample(total_import_train_dataset, max_elements, replace = FALSE)
    import_train_dataset<-import_train_dataset[train_samples,]
  }
  
  import_evaluate_dataset <- read.table(evaluate_file)
  
  datasets <- list(build = import_build_dataset,
                   train = import_train_dataset,
                   evaluate = import_evaluate_dataset)
  
  return (datasets)
}

get_som_solution <- function(x_neurons, y_neurons, topography,
                             train_elements, num_epochs, learning_rate,
                             build_elements, keep_data){
  input_vectors_matrix <- as.matrix(scale(train_elements))
  build_vectors_matrix <- as.matrix(scale(build_elements))
  
  som_grid <- somgrid(xdim = x_neurons, ydim = y_neurons, topo=topography)
  
  som_model <- som(input_vectors_matrix, som_grid,
                   rlen = num_epochs, alpha = learning_rate,
                   build_vectors_matrix, keep.data = keep_data)
  
  return (som_model)
}

get_som_exportable_values <- function(raw_data, user){
  raw_data <- cbind(raw_data, user)
  raw_data <- cbind(raw_data, 0)
  raw_data <- cbind(raw_data, 0)
  raw_data <- cbind(raw_data, 0)
  
  return(raw_data)
}

get_trained_som <- function(users, lattice_dimensions,
                            output_file_folder, dataset_file_folder,
                            train_dataset_max_size, epochs,
                            learning_rate, neighbourhood,
                            topography){
  for(user in users){
    for(dimension in lattice_dimensions){
      #print(paste(user, dimension))
      trained_som_name <- c("trained_lattice_", user, "_", dimension, "_", epochs, ".txt")
      build_dataset_input_name <- c("1", ".", user, ".txt")
      train_dataset_input_name <- c("2", ".", user, ".txt")
      evaluate_dataset_input_name <- c("3", ".", user, ".txt")
      
      trained_som_destination_path <- paste(c(output_file_folder,"/",trained_som_name), collapse = "")
      build_dataset_path <- paste(c(dataset_file_folder, "/", build_dataset_input_name), collapse = "")
      train_dataset_path <- paste(c(dataset_file_folder, "/", train_dataset_input_name), collapse = "")
      evaluate_dataset_path <- paste(c(dataset_file_folder, "/", evaluate_dataset_input_name), collapse = "")
      
      # Obtaining datasets
      datasets <- get_datasets(build_dataset_path, train_dataset_path, evaluate_dataset_path, train_dataset_max_size)
      
      # SOM algoruthm
      som_model <- get_som_solution(dimension, dimension, topography,
                                    datasets$train, epochs, learning_rate,
                                    datasets$build, TRUE)
      
      # Extract SOM trained values
      som_trained_neurons <- get_som_exportable_values(som_model$codes[[1]], user)
      
      # Export elements for C++ analysis
      write.table(som_trained_neurons, file=trained_som_destination_path, sep=" ",
                  col.names = FALSE, row.names = FALSE)
    }
  }
}